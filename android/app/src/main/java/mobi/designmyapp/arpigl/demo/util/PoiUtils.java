package mobi.designmyapp.arpigl.demo.util;

import mobi.designmyapp.arpigl.engine.impl.ArpiGlController;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Shape;
import android.graphics.Color;

/**
 * Created by Loïc Ortola on 23/08/2015.
 * This class provides utility methods to draw Pois.
 */
public abstract class PoiUtils {

    /**
     * Draw POIs around the current position.
     * @param controller the ArpiGlController instance
     */
    public static void surroundWithPois(double lat, double lon, ArpiGlController controller) {

        controller.setCameraPosition(lat, lon);

        double[] angles = {0d, Math.PI / 8, Math.PI / 4, 3 * Math.PI / 8, Math.PI / 2, 5 * Math.PI / 8, 3 * Math.PI / 4, 7* Math.PI / 8, Math.PI};

        for (double angle : angles) {
            createPoiForAngle(controller, lat, lon, angle);
            if (angle != 0d && angle != Math.PI) {
                createPoiForAngle(controller, lat, lon, -angle);
            }
        }

    }

    private static void createPoiForAngle(ArpiGlController controller, double lat, double lon, double angle) {
        final double alt = 5;
        final double deltaLat = 0.0002;
        final double deltaLon = 0.0003;

        // create a poi
        // NB : in fact, the actual poi creation is delayed until the engine use
        // it at runtime.
        Poi poi = Poi.builder()
                .shape(Shape.Default.PYRAMID)
                .icon("logo_ebusiness")
                .color(Color.BLUE)
                .position(lat + getTranslatedLat(deltaLat, angle), lon + getTranslatedLon(deltaLon, angle), alt)
                .build();
        // don't forget to add the POI to the engine to make it visible.
        controller.addPoi(poi);

        // The shape of the POI can be either of type 'Shape' for built-in
        // shapes, or you may use a String
        // to load the shape out of your own resource file. In this case, we use the Pyramid default.
        poi = Poi.builder()
                .shape(Shape.Default.PYRAMID)
                .position(lat + getTranslatedLat(deltaLat, angle), lon + getTranslatedLon(deltaLon, angle), alt)
                .altitude(8)
                .icon("logo_dma").build();
        // don't forget to add the POI to the engine to make it visible.
        controller.addPoi(poi);
    }

    private static double getTranslatedLat(double radius, double angle) {
        return Math.cos(angle) * radius;
    }

    private static double getTranslatedLon(double radius, double angle) {
        return Math.sin(angle) * radius;
    }
}
