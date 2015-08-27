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

package mobi.designmyapp.arpigl.demo.sensortrigger;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import org.json.JSONException;

import java.io.IOException;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.demo.R;
import mobi.designmyapp.arpigl.demo.util.PoiUtils;
import mobi.designmyapp.arpigl.engine.ArpiGlController;
import mobi.designmyapp.arpigl.provider.impl.OpenStreetMapTileProvider;
import mobi.designmyapp.arpigl.sensor.GravitySensorTrigger;
import mobi.designmyapp.arpigl.ui.ArpiGlFragment;


public class SensorTriggerDemoActivity extends AppCompatActivity {

    private static final int TRIGGER_VISIBLE = 30;
    private static final int TRIGGER_INVISIBLE = 80;

    private static final double SF_LAT = 37.773874;
    private static final double SF_LON = -122.4216999;

    private ArpiGlController arpiController;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // We usually use a splashscreen to do this.. But in case you don't,
        // this is your last chance to check if the resources are installed in
        // order to be able to use the MapGlController
        final ArpiGlInstaller installer = ArpiGlInstaller.getInstance(this);
        if (!installer.isInstalled()) {
            try {
                installer.install();
            } catch (final IOException | JSONException e) {
                e.printStackTrace();
            }
        }

        setContentView(R.layout.activity_demo_ar);

        // Get the arpi fragment.
        ArpiGlFragment arpiGlFragment = (ArpiGlFragment) getFragmentManager().findFragmentById(R.id.engine_fragment);

        // Create a controller to manage the arpi fragment
        arpiController = new ArpiGlController(arpiGlFragment);

        // Add the OSM tile provider to the controller
        arpiController.setTileProvider(new OpenStreetMapTileProvider());

        // this module will toggle the arpiGlView visibility, with device's tilt.
        GravitySensorTrigger visibilityTrigger = new GravitySensorTrigger(arpiGlFragment);

        // OPTIONAL SETTINGS (default values are already provided with the trigger.
        // we want the view to be visible when the tilt is below 30%,
        visibilityTrigger.setShowTreshold(TRIGGER_VISIBLE);
        // ... and to be invisible above 80%
        visibilityTrigger.setHideTreshold(TRIGGER_INVISIBLE);

        // we want the 3D View to be hidden by default. So it will display the
        // DummyFragment on the front.
        visibilityTrigger.show(false);

        // Manually add pois around current position
        PoiUtils.surroundWithPois(SF_LAT, SF_LON, arpiController);

    }

    @Override
    protected void onResume() {
        super.onResume();
        // Sets camera position
        arpiController.setCameraPosition(SF_LAT, SF_LON);
    }
}
