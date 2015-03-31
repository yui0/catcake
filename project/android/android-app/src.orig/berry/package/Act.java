package berry.@@PACKAGE@@;

import android.app.Activity;
import android.os.Bundle;
import com.kitaoworks.catcake.Catcake;


public class @@NAME@@ extends Activity
{
    private Catcake m_catcake = new Catcake("CatNative", this);

    @Override public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        m_catcake.onCreate(this);
    }

    @Override public void onPause()
    {
        super.onPause();
        m_catcake.onPause();
    }

    @Override public void onResume()
    {
        super.onResume();
        m_catcake.onResume();
    }

    @Override public void onDestroy()
    {
        super.onDestroy();
        m_catcake.onDestroy();
    }
}
