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

package mobi.designmyapp.arpigl.demo.offlineproviders;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import org.json.JSONException;

import java.io.IOException;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.demo.custompoiproviders.CustomOpenDataSoftPoiProvider;
import mobi.designmyapp.arpigl.engine.ArpiGlController;
import mobi.designmyapp.arpigl.provider.impl.AssetsStoragePoiProvider;
import mobi.designmyapp.arpigl.provider.impl.TileAssetProvider;
import mobi.designmyapp.arpigl.ui.ArpiGlFragment;
import mobi.designmyapp.arpigl.demo.R;

/**
 * This activity demonstrates the use of offline data only.
 */
public class OfflineDemoActivity extends AppCompatActivity {

    //    private static final double EIFFEL_LAT = 48.8606;
//    private static final double EIFFEL_LON = 2.2960;
    private static final double ELYSEE_LAT = 48.8708735;
    private static final double ELYSEE_LNG = 2.3036656;
//    private static final double ELYSEE_LAT = 48.870515;
//    private static final double ELYSEE_LNG = 2.305284;

    private ArpiGlController arpiController;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // We usually use a splashscreen to do this.. But in case you don't,
        // this is your last chance to check if the resources are installed in
        // order to be able to use the ArpiGlController
        final ArpiGlInstaller installer = ArpiGlInstaller.getInstance(this);
        if (!installer.isInstalled()) {
            try {
                installer.install();
            } catch (final IOException | JSONException e) {
                e.printStackTrace();
            }
        }

        setContentView(R.layout.activity_demo);

        // Get the arpi fragment.
        ArpiGlFragment arpiGlFragment = (ArpiGlFragment) getFragmentManager().findFragmentById(R.id.engine_fragment);

        // Create a controller to manage the arpi fragment.
        arpiController = new ArpiGlController(arpiGlFragment);

        arpiController.setSkyBox("TropicalSunnyDay");
        arpiController.setSkyBoxEnabled(true);

        // Add NO tile provider to the controller.
        // The default behaviour is to fetch tiles directly from your assets.
        // Your app should be bundled with a folder named 'tile' in assets/res/texture/tile

        // Add the assets poi storage provider which fetches pois from the offline offlinePois.json file, provided in assets/res
//        arpiController.addPoiProvider(new AssetsStoragePoiProvider(this, "offlinePois"));
        arpiController.addPoiProvider(new CustomOpenDataSoftPoiProvider(this));
        arpiController.setTileProvider(new TileAssetProvider("light"));
    }

    @Override
    protected void onResume() {
        super.onResume();
        // Set user location to paris, to be able to see pois
        arpiController.setCameraPosition(ELYSEE_LAT, ELYSEE_LNG);
    }
}
