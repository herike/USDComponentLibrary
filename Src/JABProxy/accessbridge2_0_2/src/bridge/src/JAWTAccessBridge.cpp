/*
 * Copyright 2005 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * @(#)JAWTAccessBridge.cpp	1.9 05/03/21
 */

/* 
 * A DLL which is loaded by Java applications and used to map
 * between Java applications and native Win32 window handles.
 */

#include "AccessBridge.h"			// programatically generated by JNI

#include <windows.h>
#include <stdio.h>

#include <jawt.h>
#include <win32/jawt_md.h>

// ---------------------------------------------------------------------------

extern "C" {
	/**	
	 * DllMain - where Windows executables will load/unload us
	 *
	 */
	BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved) {

		return TRUE;
	}


	/*
	 * Map a HWND to a Java component
	 *
	 * Class:     com_sun_java_accessibility_AccessBridge
	 * Method:    jawtGetComponentFromNativeWindowHandle
	 * Signature: (I)Ljava/awt/Component;
	 */
	JNIEXPORT jobject JNICALL 
	Java_com_sun_java_accessibility_AccessBridge_jawtGetComponentFromNativeWindowHandle
		(JNIEnv *env, jobject callingObj, jint windowHandle) {

        JAWT awt;
        jboolean result;
	    jobject component = (jobject)0;

        // Get the AWT
        awt.version = JAWT_VERSION_1_4;
        result = JAWT_GetAWT(env, &awt);
        if (result == JNI_FALSE) {
			return (jobject)0;
		}
 
        // Get the component
        return awt.GetComponent(env, (void *)windowHandle);
 	}


	/*
	 * Map a Java component to a HWND
	 *
	 * Class:     com_sun_java_accessibility_AccessBridge
	 * Method:    jawtGetNativeWindowHandleFromComponent
	 * Signature: (Ljava/awt/Component;)I
	 */
	JNIEXPORT jint JNICALL 
	Java_com_sun_java_accessibility_AccessBridge_jawtGetNativeWindowHandleFromComponent
		(JNIEnv *env, jobject callingObj, jobject component) 
	{

        JAWT awt;
        JAWT_DrawingSurface* ds;
        JAWT_DrawingSurfaceInfo* dsi;
        JAWT_Win32DrawingSurfaceInfo* dsi_win;
        jboolean result;
        // jint lock;
	    jint windowHandle = -1;

        // Get the AWT
        awt.version = JAWT_VERSION_1_4;
        result = JAWT_GetAWT(env, &awt);
        if (result == JNI_FALSE) {
			return -1;
		}
 
        // Get the drawing surface
        ds = awt.GetDrawingSurface(env, component);
        if (ds == NULL) {
			return -1;
		}
 
		/*
		 * Should not be necessary.
		 *
        // Lock the drawing surface
        lock = ds->Lock(ds);
        if ((lock & JAWT_LOCK_ERROR) != 0) {
			return -1;
		}
		 */
 
        // Get the drawing surface info
        dsi = ds->GetDrawingSurfaceInfo(ds);
 
        // Get the platform-specific drawing info
        dsi_win = (JAWT_Win32DrawingSurfaceInfo *)dsi->platformInfo;

		// Get the window handle
		windowHandle = (jint)dsi_win->hwnd;
 
        // Free the drawing surface info
        ds->FreeDrawingSurfaceInfo(dsi);
 
		/*
        // Unlock the drawing surface
        ds->Unlock(ds);
		*/
 
        // Free the drawing surface
        awt.FreeDrawingSurface(ds);

		return windowHandle;
	}
}
