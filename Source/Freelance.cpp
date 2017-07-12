
#include "Leadwerks.h"
#undef GetFileType

#include "Freelance.h"
#include "Engine.h"
#include "Utils\\Utils.h"

/*
void DebugErrorHook( char* c ) {
	cMsgCRT( "General Error:%s", c );
	int n = 0;//<--------------------------- Add a breakpoint here to catch errors
}
*/
#if _DEBUG
int main( int argc, LPCSTR argv[] ) {
#else
int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow ) {
#endif // _DEBUG

	
	HMODULE const kernel32 = LoadLibrary("kernel32.dll");
	if ( !kernel32 ) return EXIT_FAILURE;

	typedef BOOL( __stdcall*HeapSetInformation_type ) ( HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T );
	HeapSetInformation_type const heap_set_information = ( HeapSetInformation_type ) GetProcAddress( kernel32, "HeapSetInformation" );
	if (heap_set_information) {
		ULONG HeapFragValue = 2;
		heap_set_information( GetProcessHeap(),	HeapCompatibilityInformation, &HeapFragValue, sizeof( HeapFragValue ) );
	}
	
#if _DEBUG
	//Set program path
	Leadwerks::System::AppPath = Leadwerks::FileSystem::ExtractDir( argv[ 0 ] );
	
	//Load command-line parameters
	Leadwerks::System::ParseCommandLine( argc, argv );
#endif // _DEBUG

	//Add debug hook for catching errors
//	Leadwerks::System::AddHook( Leadwerks::System::DebugErrorHook, (void*)DebugErrorHook );

	Leadwerks::System::IgnoreOpenGLErrors = false;

#if defined( LAST_LE_VER )
	Leadwerks::FileSystem::SetDir( ".." );
#endif


	int ReturnValue = EXIT_FAILURE;

	if ( Engine()->IsOK() && Engine()->Initialize() ) {

		if ( Engine()->StartWorld() )
			while ( Engine()->Loop() ) { }

		delete Engine();
		
		Leadwerks::System::Shutdown();
		ReturnValue = EXIT_SUCCESS ; // OK
		cMsgDBG( "Closing.." );
	}

	return ReturnValue;
}