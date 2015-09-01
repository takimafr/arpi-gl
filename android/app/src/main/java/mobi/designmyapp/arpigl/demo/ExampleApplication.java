package mobi.designmyapp.arpigl.demo;

import android.app.Application;
import android.content.Context;

import com.squareup.leakcanary.LeakCanary;
import com.squareup.leakcanary.RefWatcher;

/**
 * Created by Loïc Ortola on 01/09/2015.
 */
public class ExampleApplication extends Application {

    @Override public void onCreate() {
        super.onCreate();
        refWatcher = LeakCanary.install(this);
    }

    public static RefWatcher getRefWatcher(Context context) {
        ExampleApplication application = (ExampleApplication) context.getApplicationContext();
        return application.refWatcher;
    }

    private RefWatcher refWatcher;
}