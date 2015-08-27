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

package mobi.designmyapp.arpigl.demo.custommarkers;

import java.io.IOException;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.demo.R;
import mobi.designmyapp.arpigl.engine.ArpiGlController;
import mobi.designmyapp.arpigl.provider.impl.AssetsStoragePoiProvider;
import mobi.designmyapp.arpigl.ui.ArpiGlFragment;

import org.json.JSONException;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;


/**
 * This Activity demonstrates the use of custom shapes for POIs.
 */
public class CustomMarkersDemoActivity extends AppCompatActivity {

    private static final double EIFFEL_LAT = 48.8606;
    private static final double EIFFEL_LON = 2.2960;

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

        // Create a controller to manage the arpi fragment
        arpiController = new ArpiGlController(arpiGlFragment);

        // Add the assets poi storage provider which fetches pois
        // from the offline customShapes.json file, provided in assets/res/pois
        arpiController.addPoiProvider(new AssetsStoragePoiProvider(this, "customShapes"));

        // Enable skybox
        arpiController.setSkyBoxEnabled(true);
    }

    @Override
    protected void onResume() {
        super.onResume();
        // Set user location to paris, to be able to see pois
        arpiController.setCameraPosition(EIFFEL_LAT, EIFFEL_LON);
    }
}
