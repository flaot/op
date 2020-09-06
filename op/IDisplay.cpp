#include "stdafx.h"
#include "IDisplay.h"
#include "globalVar.h"
#include "helpfunc.h"
IDisplay::IDisplay()
{
	_hwnd = NULL;
	_shmem = nullptr;
	_pmutex = nullptr;
	_bind_state = 0;
	_width = _height = 0;
	_client_x = _client_y = 0;

}


IDisplay::~IDisplay()
{
	bind_release();
	_bind_state = 0;
}

long IDisplay::Bind(HWND hwnd, long flag) {
	//step 1 check window exists
	if (!::IsWindow(hwnd)) {
		return 0;
	}
	_hwnd = hwnd;
	//step 2. ׼����Դ
	bind_init();
	//step 3. �����ض��İ󶨺���

	_bind_state = BindEx(hwnd, flag) ? 1 : 0;

	return _bind_state;

}

long IDisplay::UnBind() {
	//setlog("UnBind(");
	if (_bind_state) {
		UnBindEx();
	}
	bind_release();
	_bind_state = 0;
	return 1;
}

long IDisplay::bind_init() {
	int res_size = 0;
	RECT rc;
	assert(::IsWindow(_hwnd));
	::GetWindowRect(_hwnd, &rc);
	res_size = (rc.right - rc.left) * (rc.bottom - rc.top) * 4;
	wsprintf(_shared_res_name, SHARED_RES_NAME_FORMAT, _hwnd);
	wsprintf(_mutex_name, MUTEX_NAME_FORMAT, _hwnd);
	//setlog(L"mem=%s mutex=%s", _shared_res_name, _mutex_name);
	//bind_release();
	try {
		_shmem = new sharedmem();
		_shmem->open_create(_shared_res_name, res_size);
		_pmutex = new promutex();
		_pmutex->open_create(_mutex_name);
	}
	catch (std::exception& e) {
		setlog("bkdisplay::bind_init() %s exception:%s", _shared_res_name, e.what());
	}

	return 0;
}

long IDisplay::bind_release() {
	SAFE_DELETE(_shmem);
	SAFE_DELETE(_pmutex);

	_hwnd = NULL;
	//_image_data = nullptr;
	return 0;
}

//byte* bkdisplay::get_data() {
//	return _shmem->data<byte>();
//}
