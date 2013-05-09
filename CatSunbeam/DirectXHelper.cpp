#include "DirectXHelper.h"
// this function initializes and prepares Direct3D for use
void DirectXHelper::initD3D(HWND hWnd, HINSTANCE hInstance)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface
    D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information
    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = !FULLSCREEN;    // program fullscreen, not windowed
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = SCREEN_WIDTH;    // set the width of the buffer
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	v_buffer = NULL;
	
	//uncomment this to see the movement at work
	init_graphics();

	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	
	RECT rect;
	SetRect(&rect, 20, 20, SCREEN_WIDTH, 128);
	
	//tempary vectors untill rest implemented.
	camera = new CCamera();
	D3DXVECTOR3 s,m,m1,v,a;
	s.x = 0.0f;
	s.y = 3.0f;
	s.z = 3.0f;
	m.x = -3.0f;
	m.y = 0.0f;
	m.z = -3.0f;
	m1.x = 6.0f;
	m1.y = 5.0f;
	m1.z = 3.0f;
	v.x = -1.0f;
	v.y =  -.25f;
	v.z = -1.0f;
	a.x = 0.0f;
	a.y = .05f;
	a.z = 0.0f;

	textbox = new Textbox(d3ddev, 48, rect);
	input = new Input(d3ddev, camera, textbox);
	p = new Particles(s,m,m1,v,a);
	helper = new Helper();
	input->initDInput(hInstance, hWnd);
	p->intBuffers(d3ddev);

	s.x = 0.0f;
	s.y = 4.0f;
	s.z = 0.0f;
	m.x = -3.0f;
	m.y = 0.0f;
	m.z = -3.0f;
	m1.x = 3.0f;
	m1.y = 5.0f;
	m1.z = 3.0f;
	v.x = 0;
	v.y = 0;
	v.z = 0;
	a.x = 0.0f;
	a.y = .05f;
	a.z = 0.0f;

	flame = new Particles(s,m,m1,v,a);
	flame->intFlame(d3ddev);
}


// this is the function used to render a single frame
void DirectXHelper::renderFrame(void)
{
    // clear the window to a deep blue
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 115, 255), 100.0f, 0);
	//d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3ddev->BeginScene();    // begins the 3D scene

	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);

	helper->IncreaseTimer();
	textbox->SetString(helper->toString(helper->GetTime()));
	//get input and place the camera
	input->CheckForInput();
	camera->Update();
	d3ddev->SetTransform(D3DTS_VIEW, camera->GetViewMatrix());
	d3ddev->SetTransform(D3DTS_PROJECTION, camera->GetProjectionMatrix());

	textbox->Draw();
	
	// select the vertex buffer to display
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
    // do 3D rendering on the back buffer here
	// copy the vertex buffer to the back buffer
	//D3DXMATRIX matTranslate;
	//D3DXMATRIX matScale;

	//D3DXMatrixTranslation(&matTranslate, 0, 0, 0);
	//D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);

	//d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matTranslate));
    //d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	p->run_particles(d3ddev,0);
	//flame->run_particles(d3ddev,1);

    // ADDED BY ZACK
    mesh->Render(helper->GetTime(), 0);
    cat->Render(helper->GetTime(), 1);
    // END ADDED BY ZACK

    d3ddev->EndScene();    // ends the 3D scene


    d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}


// this is the function that cleans up Direct3D and COM
void DirectXHelper::cleanD3D(void)
{
    d3ddev->Release(); // close and release the 3D device
    d3d->Release(); // close and release Direct3D
	//v_buffer->Release();    // close and release the vertex buffer
	delete camera;
	delete input;
	delete textbox;
	delete p,flame;
}

void ::DirectXHelper::init_graphics(void)
{

    // ADDED BY ZACK
    mesh = new Model(d3d, d3ddev, "CatSunBeam16.x");
    cat = new Model(d3d, d3ddev, "cat2.x");
    // END ADDED BY ZACK

    // create a vertex buffer interface called v_buffer
    /*d3ddev->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &v_buffer, NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    v_buffer->Unlock();
	*/
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_SPOT;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	light.Position = D3DXVECTOR3(-1.5f, 10.0f, -15.0f);
	light.Direction = D3DXVECTOR3(0.0f, 1.0f, -4.0f);
	light.Range = 50.0f;
	light.Falloff = 1.0f;
	light.Theta = 45.0f;
	light.Phi = 45.0f;

	d3ddev->SetLight(0, &light);
	d3ddev->LightEnable(0, true);
}