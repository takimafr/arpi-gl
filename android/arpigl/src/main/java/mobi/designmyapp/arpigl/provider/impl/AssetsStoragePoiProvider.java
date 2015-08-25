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

import android.content.Context;

import java.io.File;

import mobi.designmyapp.arpigl.ArpiGlInstaller;

/**
 * Created by Loïc Ortola on 10/08/2015.
 * This Poi Storage provider scans the ArpiGl-compliant assets and map them to Pois.
 */
public class AssetsStoragePoiProvider extends PoiStorageProvider {

    public AssetsStoragePoiProvider(Context context, String datasetSid) {
        super(new File(context.getApplicationContext().getFilesDir(), ArpiGlInstaller.POIS_DIR + File.separator + datasetSid));
    }
}
