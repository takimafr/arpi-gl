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

import android.os.AsyncTask;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.UUID;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import mobi.designmyapp.arpigl.event.TileEvent;
import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.TileProvider;
import mobi.designmyapp.arpigl.util.IOUtils;

public abstract class NetworkTileProvider extends TileProvider {

    /* ***
     * CONSTANTS
     */
    private static final String TAG = NetworkTileProvider.class.getSimpleName();

    private static final int CONNECT_TIMEOUT = 15000 /* milliseconds */;
    private static final int READ_TIMEOUT = 10000 /* milliseconds */;

    private static final int CPU_COUNT = Runtime.getRuntime().availableProcessors();
    private static final int CORE_POOL_SIZE = CPU_COUNT + 1;
    private static final int MAXIMUM_POOL_SIZE = CPU_COUNT * 2 + 1;
    private static final int KEEP_ALIVE = 1;
    private static final ThreadFactory sThreadFactory = new ThreadFactory() {
        private final AtomicInteger mCount = new AtomicInteger(1);

        public Thread newThread(Runnable r) {
            return new Thread(r, "DownloadTileTask #" + mCount.getAndIncrement());
        }
    };
    private static final BlockingQueue<Runnable> sPoolWorkQueue =
            new LinkedBlockingQueue<Runnable>(128);
    private static final ThreadPoolExecutor EXECUTOR = new ThreadPoolExecutor(CORE_POOL_SIZE, MAXIMUM_POOL_SIZE, KEEP_ALIVE,
            TimeUnit.SECONDS, sPoolWorkQueue, sThreadFactory);

    private UriParser mParser;

    public interface UriParser {
        String parse(Tile.Id tile, String uri);
    }

    public NetworkTileProvider(String url) {
        super(url);
        mParser = new UriParser() {
            @Override
            public String parse(Tile.Id tile, String uri) {
                return uri
                        .replace("{x}", String.valueOf(tile.x))
                        .replace("{y}", String.valueOf(tile.y))
                        .replace("{z}", String.valueOf(tile.z));
            }
        };
    }

    @Override
    public final void fetch(Tile.Id id) {
        new DownloadTileTask().executeOnExecutor(EXECUTOR, id);
    }

    @Override
    public String getNamespace() {
        return UUID.nameUUIDFromBytes(mUri.getBytes()).toString();
    }

    public void setUriParser(UriParser parser) {
        this.mParser = parser;
    }

    private class DownloadTileTask extends AsyncTask<Tile.Id, Void, Tile> {

        @Override
        protected Tile doInBackground(Tile.Id... ids) {
            InputStream is;
            try {
                Tile.Id id = ids[0];
                String location = mParser.parse(id, mUri);

                URL url = new URL(location);
                HttpURLConnection conn;
                conn = (HttpURLConnection) url.openConnection();
                conn.setReadTimeout(READ_TIMEOUT);
                conn.setConnectTimeout(CONNECT_TIMEOUT);
                conn.setRequestMethod("GET");
                conn.setDoInput(true);
                // Starts the query
                conn.connect();
                // Response
                is = conn.getInputStream();
                // Data
                byte[] data = IOUtils.read(is);

                return new Tile(data, id);

            } catch (IOException e) {
                Log.i(TAG, "Unable to download tile " + ids[0]);
            }
            return null;
        }

        @Override
        protected void onPostExecute(Tile tile) {
            if (tile != null) {
                postEvent(new TileEvent(tile));
            }
        }
    }


}
