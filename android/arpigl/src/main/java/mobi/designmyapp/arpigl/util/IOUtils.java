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

package mobi.designmyapp.arpigl.util;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

/**
 * Created by Loïc Ortola on 10/08/2015.
 */
public final class IOUtils {

    private static final int READ_BUFFER = 8192;

    /**
     * Default Constructor.
     */
    private IOUtils() {

    }

    /**
     * Store content of the the given inputstream into a byte array.
     *
     * @param input the input stream
     * @return the resulting byte array.
     * @throws IOException if an error occurs while reading the given inputStream.
     */
    public static byte[] read(InputStream input) throws IOException {
        byte[] buffer = new byte[READ_BUFFER];
        int bytesRead;
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        while ((bytesRead = input.read(buffer)) != -1) {
            output.write(buffer, 0, bytesRead);
        }
        return output.toByteArray();
    }

    /**
     * Copy the source file into the destination file.
     *
     * @param src the source file.
     * @param dst the destination file.
     * @throws IOException Signals that an I/O exception has occurred.
     */
    public static void copy(File src, File dst) throws IOException {
        InputStream in = new FileInputStream(src);
        OutputStream out = new FileOutputStream(dst);

        // Transfer bytes from in to out
        byte[] buf = new byte[READ_BUFFER];
        int len;
        while ((len = in.read(buf)) > 0) {
            out.write(buf, 0, len);
        }
        in.close();
        out.close();
    }

    public static JSONObject toJsonObject(InputStream jsonFile, String encoding) throws IOException, JSONException {

        BufferedReader br = null;
        try {
            br = new BufferedReader(new InputStreamReader(jsonFile, encoding));

            final StringBuilder sb = new StringBuilder();

            String inputStr;
            while ((inputStr = br.readLine()) != null) {
                sb.append(inputStr);
            }

            return new JSONObject(sb.toString());

        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e2) {
                    e2.printStackTrace();
                }
            }
        }
    }
}
