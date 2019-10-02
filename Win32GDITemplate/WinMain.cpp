#include <Windows.h>  // do not need everything from here

HWND hwnd;				//<--- A handle to the window.
RECT clientRect;		//<--- A struct describing a rectangle that will be the client space of the window.
int w, h;				//<--- The width and height of the window.
float x = 0;			//<--- X position of the "player".

//Forward declaration of custon windows procedure function.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// WinMain is the main function for Win32 Applications.
// Parameters are mostly historical.
// 1 - Handle to a structure that holds information about the current instance of the application.
// 2 - Handle to a structure that holds information about the previous instance of the application. This is allways NULL and is only there for backward compatibility.
// 3 - A char pointer that holds a string (command line) passed to the application. 
// 4 - An int used to indicate to the application how the window should be shown.
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const char* className = "Win32GDITemplate";

	// WNDCLASS is a struct that contains a windows class attributes.
	// = { 0 } initializes all members to 0;
	WNDCLASS wc = { 0 };
	// lpfnWndProc is a pointer to the windows procedure that is called when DispatchMessage() is called.
	wc.lpfnWndProc = WindowProc;
	// hInstance is a handle to the instance of the application that contains the windows procedure. (This instance.)
	wc.hInstance = hInstance;
	// lpszClassName is a string that identifies the windows class.
	wc.lpszClassName = className;
	// hbrBackground is the HBRUSH used to "paint" the background of the client window.
	// HBRUSH is a handle to a GDI brush.
	// GDI (Graphics Device Interface) is a Win32 API and core operating system component responsible for representing graphical objects.
	// hbrBackground is commented out as the WM_ERASEBKGND windows message is overridden in the custom windows procedure so it is never used.
	//wc.hbrBackground = (HBRUSH)BLACK_BRUSH;

	// RegisterClass() registers a window class for subsequent use in calls to CreateWindow().
	RegisterClass(&wc);

	// CreateWindow() creates a window using the registered WNDCLASS struct.
	// 1 - string to identify the windows class to be created.
	// 2 - string that will be displayed in the title bar of the window.
	// 3 - A 32 bit integer identifying the style of the window. Styles can be bitwise ORed together to combine styles.
	//     WS_OVERLAPPEDWINDOW has a title bar and a border.
	// 4 - X position of window.
	// 5 - Y position of window.
	// 6 - Width of window.
	// 7 - Height of window.
	// 8 - A HWND handle to the parent window.
	// 9 - A HMENU handle to a struct that defines the attributes of a windows menu.
	// 10 - A handle to the instance of the application.
	// 11 - A LPVOID (void) pointer to the LPARAM passed to the window through the WM_CREATE windows message that is called before CreateWindow() returns.
	// CreateWindow() returns a handle to the window that is stored for later use.
	hwnd = CreateWindow(
		className,
		"",
		WS_OVERLAPPEDWINDOW,
		100, 100,
		1000, 700,
		NULL,
		NULL,
		hInstance,
		NULL);

	// GetClientRect stores a RECT struct representing the windows client area of the window.
	// 1 - A handle to the targeted window.
	// 2 - A RECT pointer to store the client RECT.
	GetClientRect(hwnd, &clientRect);
	w = clientRect.right - clientRect.left;		//<--- Calculating the width of the client window.
	h = clientRect.bottom - clientRect.top;		//<--- Calculating the height of the client window.

	// Displays the window.
	// 1 - A handle to the window.
	// 2 - An int used to indicate to the application how the window should be shown. 
	//     This is ignored on first call to ShowWindow().
	ShowWindow(hwnd, nCmdShow);

	// A windows message struct initialized to 0.
	MSG msg = { 0 };
	while (true)
	{
		// PeekMessage() retrieves a windows message from the message queue if one exists.
		// 1 - A pointer to the MSG struct to store the message in.
		// 2 - A HWND handle to the window whose messages are to be retrieved.
		//	   If nullptr, then messages will be recieved from all windows.
		// 3 - An UINT (unsigned int) that is the value of the first message in the range of messages to be examined. 
		// 4 - An UINT (unsigned int) that is the value of the last message in the range of messages to be examined. 
		//     If parameters 3 and 4 are both 0 then PeekMessage() returns all messages.
		// 5 - An UINT (unsigned int) flag that specifies what to do with the message once it is retrieved. 
		//     PM_REMOVE discards the message.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// TranslateMessage() translate virtual key messages (like VM_KEYDOWN) into character messages.
			TranslateMessage(&msg);
			// DispatchMessage dispatches a message to the defined window procedure.
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			x += 1.0f;

			// InvalidateRect adds a rectangle in the windows update region and then sends a WM_PAINT message to the window.
			// The windows update region is the region of the client window that needs to be updated.
			// 1 - A handle to the targeted window.
			// 2 - A pointer to the RECT to invalidate.
			// 3 - A boolean to determine if the RECT is deleted or just left un-changed.
			InvalidateRect(hwnd, &clientRect, TRUE);
		}
	}

	return 0;
}

// Custom defined windows procedure function.

// LRESULT is a typedef for LONG_PTR
// LONG_PTR is a typedef for __int3264
// __int3264 is a 32 bit int on 32 bit systems and a 64 bit int on 64 bit systems.

// CALLBACK is a function modifier.
// CALLBACK is defined as __stdcall.
// __stdcall is a calling convention.
// A calling convention specifies how a function call in C or C++ is converted into assembly language.
// __stdcall tells c++ to use this calling convention instead of the standard calling convention, __cdecl. 
// __stdcall passes parameters on the stack differently.
// __stdcall is used to pass Win32 functions.
// Allways use _stdcall when writing functions that the Win32 API has to call.

// 1 - Handle to a window.
// 2 - Unsigned int representing windows message.
// 3 - WPARAM is a typedef for UINT_PTR.
//	   UINT_PTR is a typedef for unsigned int.
//	   WPARAM is an optional parameter that is used for different purposes for different windows messages.
// 4 - LPARAM is a typedef for LONG_PTR.
//     LONG_PTR is a typedef for long.
//	   LPARAM is an optional parameter that is used for different purposes for different windows messages.
// (W stands for "word" and L stands for "long". Historically a word was 16 bits and long was 32 bits.)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Steps to implement a double buffered solution:
	// Instructions taken from https://www.gamedev.net/forums/topic/411559-win32-double-buffering/

	// 1. Use the CreateCompatibleDC function to create a memory device context
	// 	  that is compatible with the target display device.

	// 2. Create an appropriately sized device - dependent bitmap or DIB section bitmap
	//    that will serve as the "memory" of the memory device context.
	//    You can use the CreateCompatibleBitmap function to create a device - dependent bitmap
	//    or the CreateDIBSection function to create a DIB section bitmap.

	// 3. Use the SelectObject function to select the bitmap into the memory device context.

	// 4. Direct any rendering to the memory device context.You can simply modify your existing
	//    code by replacing all references to the original target device context with references
	//    to the memory device context.

	// 5. When the rendering is complete, use the BitBlt function to transfer the bits of the bitmap
	//    to the original target device context.

	// Windows messages are processed here.

	switch (msg)
	{
		// WM_PAINT is called every time InvalidateRect() is called in the "Game loop".
	case WM_PAINT:
	{
		// PAINTSTRUCT is a struct that contains information that can be used to "paint" the client area of a window.
		PAINTSTRUCT ps;

		// HDC is a handle to a display device context.
		// A device context is a structure that defines a set of graphic objects and their associated attributes, as well as the graphic modes that affect output.
		// Graphic objects are objects used to "paint" with.
		// Creating two device contexts to implement double buffering to eliminate flickering.
		HDC hdcFront;
		HDC hdcBack;

		// BeginPaint() prepares the specified window for painting and fills a PAINTSTRUCT struct with information about the painting.
		// BeginPaint() returns a handle to a device context for the specified window.
		// The front buffer is set to this device context.
		hdcFront = BeginPaint(hwnd, &ps);

		// CreateCompatibleDC() creates a device context compatible with the specified device. It has the same attributes.
		// 1 - The HDC to be compatible with.
		// The back buffer is set to this device context.
		hdcBack = CreateCompatibleDC(hdcFront);

		// HBITMAP is a handle to a GDI bitmap class.
		// GDI (Graphics Device Interface) is a Win32 API and core operating system component responsible for representing graphical objects.
		// CreateCompatibleBitmap creates a bitmap compatible with a specified device context.
		// 1 - The HDC to be compatible with.
		// 2 - The width of the bitmap.
		// 3 - The height of the bitmap.
		HBITMAP Backbitmap = CreateCompatibleBitmap(hdcFront, w, h);

		// SelectObject() replaces the object of the specified type in the specified device context.
		// 1 - The targeted device context.
		// 2 - The HGDIOBJ (GDI graphic object) object to be selected.
		//	   HBITMAP is a valid HGDIOBJ object.
		SelectObject(hdcBack, Backbitmap);


		// HBRUSH is a GDI graphic object used for filling polygons.
		// CreateSolidBrush() creates an HBRUSH.
		// 1 - The colour of the brush.
		HBRUSH Brush = CreateSolidBrush(RGB(0, 0, 255));
		// Selecting the brush to be used on the back graphic device context.
		SelectObject(hdcBack, Brush);
		// Rectangle() "draws" a rectangle to the targeted device context.
		// 1 - The targeted device context.
		// 2 - Top-left X position.
		// 3 - Top-left Y position.
		// 4 - Bottim-right X position.
		// 5 - Bottim-right X position.
		Rectangle(hdcBack, x, 50, x + 40, 40);


		// BitBlt transfers the colour values of one device context onto another.
		// This is where the back and front buffers are flipped in double buffering.
		// 1 - Device context to transfer to.
		// 2 - X co-ordinate of destination RECT.
		// 3 - Y co-ordinate of destination RECT.
		// 4 - Width of destination RECT.
		// 5 - Height of destination RECT.
		// 6 - Device context to transfer from.
		// 7 - X co-ordinate of source RECT.
		// 8 - Y co-ordinate of source RECT.
		// 9 - An unsigned long defining a raster-operation code. 
		//	   These codes define how the color data for the source rectangle is to be combined with the color data for the destination rectangle to achieve the final color.
		// SRCCOPY copies the source rectangle directly to the destination rectangle. 	   
		BitBlt(hdcFront, 0, 0, w, h, hdcBack, 0, 0, SRCCOPY);

		// Deleting objects and device contexts
		DeleteObject(Backbitmap);
		DeleteObject(Brush);
		DeleteDC(hdcBack);
		DeleteDC(hdcFront);

		// EndPaint() marks the end of painting in the specified window.
		EndPaint(hwnd, &ps);
		return 0;
	}
	break;
	// WM_ERASEBKGND is called whenever WM_PAINT is. 
	// The default functionality in DefWindowProc() is to erase the client window background or to fill it with the specified HBRUSH defined in hbrBackground.
	// This is overwritten to avoid painting to the client window twick a frame, creating a flickering effect. 
	case WM_ERASEBKGND:
		return 0;
		// WM_CLOSE is called when the user presses the close button on the window.
	case WM_CLOSE:
		// PostQuitMessage() posts a WM_QUIT message to the thread's message queue.
		// The WM_QUIT message is handled in the "game loop", before the windows procedure function.
		PostQuitMessage(0);
		return 0;
	}
	// If the message is not any of the above, call the default windows procedure.
	// This handles around 1000 other potential windows messages.
	return DefWindowProc(hwnd, msg, wParam, lParam);
}



