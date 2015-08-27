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

package mobi.designmyapp.arpigl;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import mobi.designmyapp.arpigl.engine.ArpiGlController;
import mobi.designmyapp.arpigl.mapper.DefaultInputStreamPoiMapper;
import mobi.designmyapp.arpigl.mapper.DefaultPoiJsonMapper;
import mobi.designmyapp.arpigl.mapper.PoiMapper;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.util.ProjectionUtils;

/**
 * GeoScenes may come bundled with custom tiles or POI. This installer will
 * parse pois.json file, and split POI definition to sort them in sub-folders,
 * to be able to fetch them by tile.
 *
 * @author Nicolas THIERION.
 */
class PoiInstaller {



    /* ***
     * CONSTRUCTOR
     */
    public PoiInstaller() {
    }

    /* ***
     * PUBLIC METHODS
     */

    /**
     * Installs the provided pois onto the storage. Installation process parse
     * all JSON located in {@code INSTALLATION_DIR}, & write all poi found in
     * sub json files classed by tile in sub-folders.
     *
     * @throws IOException
     * @throws JSONException
     * @param poisDir where to install pois
     */
    public boolean install(File poisDir) throws IOException, JSONException {
        return install(poisDir, new DefaultInputStreamPoiMapper());
    }

    /**
     * Installs the provided pois onto the storage. Installation process parse
     * all JSON located in {@code INSTALLATION_DIR}, & write all poi found in
     * sub json files classed by tile in sub-folders.
     *
     * @param poisDir where to install pois
     * @param mapper a custom Poi Mapper
     * @throws IOException
     * @throws JSONException
     */
    public boolean install(File poisDir, PoiMapper<InputStream> mapper) throws IOException, JSONException {

        List<String> filenames = new LinkedList<String>();
        FilenameFilter jsonFF = new FilenameFilter() {

            @Override
            public boolean accept(File dir, String filename) {
                return filename.endsWith(".json");
            }
        };


        listFiles(poisDir.getAbsolutePath(), filenames, jsonFF);

        // Open each JSON in in resource folder
        for (String filename : filenames) {
            // Contains poi definitions, classed by tiles. Poi must have zoom attribute to be assigned to a tile.
            Map<Tile.Id, List<Poi>> poiMap = new HashMap<>();
            File file = new File(filename);
            InputStream currentJsonStream = new FileInputStream(file);
            List<Poi> pois = mapper.convert(currentJsonStream);
            sortByTile(poiMap, pois);

            // Write Tiles to JSON files in internal storage
            String datasetName = file.getName();
            datasetName = datasetName.substring(0, datasetName.length() - ".json".length());
            writeTiles(poisDir, poiMap, datasetName);
        }



        // Everything went fine, delete files
        clearFiles(filenames);


        return true;
    }

    private void sortByTile(Map<Tile.Id, List<Poi>> poiMap, List<Poi> pois) {
        for (Poi poi : pois) {
            // Generate tile ID.
            int z = ArpiGlController.MAP_ZOOM;
            int x = ProjectionUtils.lng2tilex(poi.getLongitude(), z);
            int y = ProjectionUtils.lat2tiley(poi.getLatitude(), z);
            Tile.Id tid = new Tile.Id(x, y, z);
            List<Poi> poiList = poiMap.get(tid);
            if (poiList == null) {
                poiList = new LinkedList<>();
                poiMap.put(tid, poiList);
            }
            poiList.add(poi);
        }
    }

    private void writeTiles(File poisDir, Map<Tile.Id, List<Poi>> poiMap, String datasetName) throws IOException {
        for (Map.Entry<Tile.Id, List<Poi>> entry : poiMap.entrySet()) {
            DefaultPoiJsonMapper poiJsonMapper = new DefaultPoiJsonMapper();
            JSONObject tile = poiJsonMapper.convert(entry.getValue());

            File destFile = new File(poisDir, datasetName);
            File target = new File(destFile, entry.getKey().toString() + ".json");
            target.getParentFile().mkdirs();
            target.getParentFile().createNewFile();
            FileWriter writer = new FileWriter(target);
            writer.write(tile.toString());
            writer.close();
        }
    }

    private void clearFiles(List<String> filenames) {
        // JSON files have been parsed... clean resource dir before writing new json files
        for (String filename : filenames) {
            File file = new File(filename);
            file.delete();
            File dir = file.getParentFile();
            if (dir.isDirectory() && dir.list().length == 0) {
                dir.delete();
            }
        }
    }

    /* ***
     * PRIVATE METHODS
     */

    /**
     * List the files recursively in all folders & subfolders of filesystem,
     * that matches the given {@link FilenameFilter}.
     *
     * @param path           The path to search.
     * @param fileList       the list of files within the "path" hierarchy.
     * @param filenameFilter the filenameFilter to match.
     * @return the estimated size in bytes of the folder's content.
     * @throws IOException if an error occurs during reading the files.
     */

    private long listFiles(String path, List<String> fileList, FilenameFilter filenameFilter) throws IOException {
        long sizeInBytes = 0;
        File file = new File(path);

        if (file.isDirectory()) {
            String[] list;
            list = new File(path).list();

            // This is a folder
            for (final String filename : list) {
                final long currentSize = listFiles(path + "/" + filename, fileList, filenameFilter);
                if (currentSize < 0) {
                    return currentSize;
                }
                sizeInBytes += currentSize;
            }
        } else {
            if (filenameFilter.accept(file.getParentFile(), path)) {
                fileList.add(path);
                final InputStream is = new FileInputStream(file);
                sizeInBytes = is.available();
                is.close();
            }
        }

        return sizeInBytes;
    }
}
