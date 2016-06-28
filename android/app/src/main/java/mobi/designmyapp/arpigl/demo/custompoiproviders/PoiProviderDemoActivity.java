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

package mobi.designmyapp.arpigl.demo.custompoiproviders;

import java.io.IOException;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.demo.R;
import mobi.designmyapp.arpigl.engine.ArpiGlController;
import mobi.designmyapp.arpigl.listener.PoiSelectionListener;
import mobi.designmyapp.arpigl.provider.impl.OpenStreetMapTileProvider;
import mobi.designmyapp.arpigl.ui.ArpiGlFragment;

import org.json.JSONException;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.Toast;

/**
 * This activity demonstrates the use of multiple Poi Providers, local (from assets), or remote (from OpenDataSoft.com)
 * Any custom Poi Provider can be added this way.
 */
public class PoiProviderDemoActivity extends AppCompatActivity {

    private static final double PARIS_CENTER_LAT = 48.8554;
    private static final double PARIS_CENTER_LONG = 2.3474;


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

        // Add the OSM tile provider to the controller
        arpiController.setTileProvider(new OpenStreetMapTileProvider());

        // Add our custom Poi Providers
        // This one will download pois from one of the opendatasoft.com
        // public datasets
        arpiController.addPoiProvider(new CustomOpenDataSoftPoiProvider(this));

        // This one will fetch pois (food) from the google places API
        arpiController.addPoiProvider(new CustomGooglePlacesPoiProvider("AIzaSyBULbG_upfRjG5nrh9x5MSaBbc6lIWilHU", this));

        // Set the poi selection listener
        arpiController.setPoiSelectionListener(new PoiSelectionListener() {
            @Override
            public void onPoiSelected(String sid) {
                Toast.makeText(PoiProviderDemoActivity.this, "Poi " + sid + " selected", Toast.LENGTH_SHORT).show();
            }
            @Override
            public void onPoiDeselected(String sid) {
                Toast.makeText(PoiProviderDemoActivity.this, "Poi " + sid + " deselected", Toast.LENGTH_SHORT).show();
            }
        });

        // Enable Skybox
        arpiController.setSkyBoxEnabled(true);

    }

    @Override
    protected void onResume() {
        super.onResume();
        // Disable location tracking, so we can build a local scene you can see,
        // no matter what is your actual position. Note : this is disabled by default.
        arpiController.setUserLocationEnabled(false);

        // Set user location to paris, to be able to see pois
        arpiController.setCameraPosition(PARIS_CENTER_LAT, PARIS_CENTER_LONG);
    }
}
