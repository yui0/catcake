package berry.catcake;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.view.MotionEvent;
import android.widget.LinearLayout.LayoutParams;


public class Catcake
{
	private static native void nativeInitialize(AssetManager assetManager, int width, int height);
	private static native void nativeUpdate();
	private static native void nativeFinalize();
	private static native void nativePause();
	private static native void nativeResume();
	private static native void nativeTouch(int action, int x, int y);
	//private static native void nativeOnKeyDown(int key_code);
	//private static native void nativeOnKeyUp(int key_code);

	public static final int APP_TYPE = 1;

	private Context m_context;
	private CatcakeView m_view;
	private CatcakeRenderer m_renderer;
	private CameraView mCameraView;

	public Catcake(String lib_name, Context context)
	{
		System.loadLibrary(lib_name);
		m_context = context;
	}

	public void onCreate(Activity activity)
	{
		activity.getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);
		activity.requestWindowFeature(android.view.Window.FEATURE_NO_TITLE);

		m_view = new CatcakeView(activity);
		m_renderer = new CatcakeRenderer();
		m_view.setRenderer(m_renderer);

		if (APP_TYPE == 0) {
			// Normal
			activity.setContentView(m_view);
		} else {
			// Support Camera
			activity.getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
			mCameraView = new CameraView(m_context);
			activity.setContentView(mCameraView);
			LayoutParams params = new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
			activity.addContentView(m_view, params);
			m_view.setZOrderOnTop(true);
		}
	}

	public void onPause()
	{
		m_view.onPause();
		nativePause();
	}

	public void onResume()
	{
		m_view.onResume();
		nativeResume();
	}

	public void onDestroy()
	{
		nativeFinalize();
	}

	private class CatcakeView extends GLSurfaceView
	{
		public CatcakeView(Context context)
		{
			super(context);
		}

		@Override public void setRenderer(Renderer renderer)
		{
			super.setRenderer(renderer);
		}

		@Override public boolean onTouchEvent(MotionEvent event)
		{
			nativeTouch(event.getAction(), (int)event.getX(), (int)event.getY());
			return true;
		}
	}

	private class CatcakeRenderer implements Renderer
	{
		boolean m_is_first = true;

		public void onSurfaceCreated(GL10 gl, EGLConfig config) {}

		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			if (m_is_first)
			{
				m_is_first = false;

				nativeInitialize(m_context.getAssets(), width, height);
			}
		}

		public void onDrawFrame(GL10 gl)
		{
			/*if (m_is_first)
			{
				m_is_first = false;

				nativeInitialize(m_context.getAssets());
			}*/

			nativeUpdate();
		}
	}
}
