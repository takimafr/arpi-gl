/*
 * Copyright (C) 2015  eBusiness Information
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package mobi.designmyapp.arpigl.provider.impl;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import mobi.designmyapp.arpigl.event.PoiEvent;
import mobi.designmyapp.arpigl.exception.PoiMapperException;
import mobi.designmyapp.arpigl.mapper.PoiMapper;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.PoiProvider;
import mobi.designmyapp.arpigl.util.ProjectionUtils;
import android.os.AsyncTask;
import android.util.Log;

public class NetworkPoiProvider extends PoiProvider<InputStream> {

    /* ***
     * CONSTANTS
     */
    private static final String TAG = NetworkPoiProvider.class.getSimpleName();

    private static final int CONNECT_TIMEOUT = 15000 /* milliseconds */;
    private static final int READ_TIMEOUT = 10000 /* milliseconds */;

    private static final int CPU_COUNT = Runtime.getRuntime().availableProcessors();
    private static final int CORE_POOL_SIZE = CPU_COUNT + 1;
    private static final int MAXIMUM_POOL_SIZE = CPU_COUNT * 2 + 1;
    private static final int KEEP_ALIVE = 1;
    private static final ThreadFactory sThreadFactory = new ThreadFactory() {
        private final AtomicInteger mCount = new AtomicInteger(1);

        @Override
        public Thread newThread(Runnable r) {
            return new Thread(r, "DownloadPoiTask #" + mCount.getAndIncrement());
        }
    };
    private static final BlockingQueue<Runnable> sPoolWorkQueue =
            new LinkedBlockingQueue<>(512);
    private static final ThreadPoolExecutor EXECUTOR = new ThreadPoolExecutor(CORE_POOL_SIZE, MAXIMUM_POOL_SIZE, KEEP_ALIVE,
            TimeUnit.SECONDS, sPoolWorkQueue, sThreadFactory);

    private Map<Tile.Id, Set<Poi>> mPoiCache;
    private UriParser mParser;

    /**
     * Custom loader for network pois.
     */
    public interface UriParser {

        /**
         * Parses the given uri to replace placeholders against the given
         * Tile.id.
         *
         * @param tile the tile
         * @param uri the uri
         * @return the string
         */
        String parse(Tile.Id tile, String uri);
    }


    public NetworkPoiProvider(String url, Class<? extends PoiMapper<InputStream>> poiMapperClass) {
        super(url, poiMapperClass);

        mPoiCache = new LinkedHashMap<Tile.Id, Set<Poi>>() {
            private static final long serialVersionUID = 4251618832889955850L;
            private static final int MAX_ENTRIES = 25;

            @Override
            protected boolean removeEldestEntry(Entry<Tile.Id, Set<Poi>> eldest) {
                return size() > MAX_ENTRIES;
            }
        };

        mParser = new UriParser() {
            @Override
            public String parse(Tile.Id tile, String uri) {

                double[] coords1 = ProjectionUtils.tile2latLon(tile);
                double[] coords2 = ProjectionUtils.tile2latLon(tile.x + 1, tile.y + 1, tile.z);
                double[] coords = {(coords1[0] + coords2[0]) / 2, (coords1[1] + coords2[1]) / 2};

                // As we have 5x5 tiles, the radius of the center tile will be multiplied by 5.
                double radius = 5 * ProjectionUtils.circumscribedCircleRadiusInMeters(tile, 19);

                return uri.replace("{lat}", ProjectionUtils.COORDINATES_FORMAT.format(coords[0]))
                        .replace("{lon}", ProjectionUtils.COORDINATES_FORMAT.format(coords[1]))
                        .replace("{radius}", String.valueOf(radius));
            }
        };
    }

    @Override
    public final void fetch(Tile.Id id) {
        Set<Poi> pois = mPoiCache.get(id);
        if (pois != null) {
            postEvent(new PoiEvent(pois));
        } else {
            new DownloadPoiTask().executeOnExecutor(EXECUTOR, id);
        }
    }

    public void setUriParser(UriParser parser) {
        this.mParser = parser;
    }

    private class DownloadPoiTask extends AsyncTask<Tile.Id, Void, Set<Poi>> {

        @Override
        protected Set<Poi> doInBackground(Tile.Id... ids) {
            InputStream is;
            try {

                String location = mParser.parse(ids[0], getUri());

                URL url = new URL(location);

                HttpURLConnection conn;
                conn = (HttpURLConnection) url.openConnection();
                conn.setReadTimeout(READ_TIMEOUT);
                conn.setConnectTimeout(CONNECT_TIMEOUT);
                conn.setRequestMethod("GET");
                conn.setDoInput(true);
                // Starts the query
                conn.connect();

                is = conn.getInputStream();
                return convert(is);
            } catch (IOException e) {
                Log.e(TAG, "Unable to download pois for tile :  " + ids[0] + " : " + e.getMessage());
            } catch (PoiMapperException e) {
                Log.e(TAG, "Unable to convert poi :  " + ids[0] + " : " + e.getMessage());
            }
            return null;
        }


        @Override
        protected void onPostExecute(Set<Poi> pois) {
            if (pois != null) {
                postEvent(new PoiEvent(pois));
            }
        }
    }

}
