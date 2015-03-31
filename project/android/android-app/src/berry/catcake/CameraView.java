package berry.catcake;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.Size;
import android.os.Environment;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;


public class CameraView extends SurfaceView implements SurfaceHolder.Callback, /*Camera.PictureCallback, */AutoFocusCallback {
//	public native void nativeCameraInitialize();
	public native static void nativeCameraFrame(byte[] yuv420sp, int width, int height);
//	public native void nativeCameraFinalize();

	private SurfaceHolder mHolder;
	private static Camera mCamera;	// static でないと NDK から使えない

	//private int sx, sy;
	private static int cwidth = 640, cheight = 480;

	private final static Camera.PreviewCallback mCallback = new Camera.PreviewCallback() {
		public void onPreviewFrame(byte[] data, Camera camera) {
			nativeCameraFrame(data, cwidth, cheight);
			mCamera.setOneShotPreviewCallback(mCallback);
		}
	};
	public void StartPreview() {
		//Log.d("CameraView", "StartPreview");
		if (mCamera==null) return;
		mCamera.startPreview();
		mCamera.setOneShotPreviewCallback(mCallback);
	}
	public void StopPreview() {
		if (mCamera==null) return;
		mCamera.stopPreview();
	}
	public void FlashOn() {
		Log.d("CameraView", "FlashOn");
		if (mCamera==null) return;
		Log.d("CameraView", "FlashOn++");
		Camera.Parameters params = mCamera.getParameters();
		//params.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
		params.setFlashMode(Camera.Parameters.FLASH_MODE_TORCH/*"torch"*/);
		//params.setFlashMode(Camera.Parameters.FLASH_MODE_ON);
		mCamera.setParameters(params);
	}
	public void FlashOff() {
		if (mCamera==null) return;
		Camera.Parameters params = mCamera.getParameters();
		params.setFlashMode("off");
		mCamera.setParameters(params);
	}
	public void OpenCamera(int id) {
		try {
			mCamera = Camera.open(id);
			mCamera.setPreviewDisplay(null);

			Camera.Parameters parameters = mCamera.getParameters();
			//List<size> s = parameters.getSupportedPreviewSizes();
			parameters.setPreviewSize(cwidth, cheight);
			mCamera.setParameters(parameters);
		} catch (Exception e) {
		}
		StartPreview();
	}
	public void CloseCamera() {
		if (mCamera!=null) {
			mCamera.setPreviewCallback(null);
			mCamera.stopPreview();
			mCamera.release();
			mCamera = null;
		}
	}
	public void SetCameraId(int id) {
		//Log.d("CameraView", "CameraID "+id);
		CloseCamera();
		OpenCamera(id);
		/*Camera.Parameters params = mCamera.getParameters();
		params.set("camera-id", id);
		mCamera.setParameters(params);*/
	}

	// コンストラクタ
	public CameraView(Context context) {
		super(context);
//		nativeCameraInitialize();

		// サーフェイスホルダーの生成
		mHolder = getHolder();
		mHolder.addCallback(this);

		// プッシュバッッファの指定
		mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		//mHolder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);
	}

	// サーフェイス生成イベントの処理
	public void surfaceCreated(SurfaceHolder holder) {
		// カメラの初期化
		OpenCamera(0);
		/*try {
			mCamera = Camera.open();
			mCamera.setPreviewDisplay(null);

			Camera.Parameters parameters = mCamera.getParameters();
			//List<size> s = parameters.getSupportedPreviewSizes();
			parameters.setPreviewSize(cwidth, cheight);
			mCamera.setParameters(parameters);
		} catch (Exception e) {
		}

		StartPreview();*/
	}
	// サーフェイス変更イベントの処理
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// 描画データの準備
		/*sx = w;
		sy = h;

		Camera.Parameters parameters = mCamera.getParameters();
		parameters.setPreviewSize(cwidth, cheight);
		mCamera.setParameters(parameters);

		// カメラプレビューの開始
		mCamera.startPreview();
		mCamera.setOneShotPreviewCallback(mCallback);*/
	}
	// サーフェイス解放イベントの処理
	public void surfaceDestroyed(SurfaceHolder holder) {
//		nativeCameraFinalize();

		// カメラのプレビュー停止
		CloseCamera();
		/*if (mCamera!=null) {
			mCamera.setPreviewCallback(null);
			mCamera.stopPreview();
			mCamera.release();
			mCamera = null;
		}*/
	}

	public void onAutoFocus(boolean success, Camera camera) {
		if (!success) return;
		//camera.takePicture(null, null, pictureCallback);
	}
}
