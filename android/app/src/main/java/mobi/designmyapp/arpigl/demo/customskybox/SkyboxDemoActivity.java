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

package mobi.designmyapp.arpigl.demo.customskybox;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

import org.json.JSONException;

import java.io.IOException;
import java.util.Iterator;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.demo.util.PoiUtils;
import mobi.designmyapp.arpigl.engine.impl.ArpiGlController;
import mobi.designmyapp.arpigl.provider.impl.OpenStreetMapTileProvider;
import mobi.designmyapp.arpigl.ui.ArpiGlFragment;
import mobi.designmyapp.arpigl.demo.R;


public class SkyboxDemoActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String[] skyboxes = {"CloudyLightRays", "DarkStormy", "FullMoon", "SunSet", "ThickCloudsWater", "TropicalSunnyDay"};

    private static final double CALI_LAT = 34.221595;
    private static final double CALI_LON = -119.0371413;

    private ArpiGlController arpiController;
    private Button skyboxBtn;
    private int currentIndice = 0;

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

        setContentView(R.layout.activity_demo_skybox);

        // Get the arpi fragment.
        ArpiGlFragment arpiGlFragment = (ArpiGlFragment) getFragmentManager().findFragmentById(R.id.engine_fragment);

        skyboxBtn = (Button) findViewById(R.id.btn_skybox);
        skyboxBtn.setOnClickListener(this);

        // Create a controller to manage the arpi fragment
        arpiController = new ArpiGlController(arpiGlFragment);

        // Add the OSM tile provider to the controller
        arpiController.setTileProvider(new OpenStreetMapTileProvider());

        // We enable skybox, & we tell to use the skybox "SunSet" provided in
        // assets/arpigl/texture/cubemap/skybox
        arpiController.setSkyBox(skyboxes[0]);
        arpiController.setSkyBoxEnabled(true);

        // Manually add pois around current position
        PoiUtils.surroundWithPois(CALI_LAT, CALI_LON, arpiController);
    }

    @Override
    protected void onResume() {
        super.onResume();
        // Sets camera position
        arpiController.setCameraPosition(CALI_LAT, CALI_LON);
    }

    @Override
    public void onClick(View v) {
        currentIndice = (currentIndice + 1) % skyboxes.length;
        arpiController.setSkyBox(skyboxes[currentIndice]);
    }
}
