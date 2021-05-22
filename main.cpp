#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkPolyData.h>
#include <vtkOpenGLActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkAutoInit.h>
#include <vtkShaderProperty.h>
#include <vtkUniforms.h>
#include <vtkMatrix4x4.h>
#include <vtkMatrix3x3.h>
#include <vtkCellPicker.h>
#include <vtkSphereSource.h>
#include <vtkPLYReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>
#include <vtkPoints.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

//Vtk Global Variables
vtkSmartPointer<vtkRenderWindowInteractor> iren;
vtkSmartPointer<vtkRenderWindow> renWin;
vtkSmartPointer<vtkRenderer> ren;
vtkSmartPointer<vtkActor> currentActor;
vtkSmartPointer<vtkActor> sphere;
vtkSmartPointer<vtkCellPicker> picker;


//Shader Texts
std::string vertexShaderText;
std::string fragmentShaderText;


//Observer Function
void onMouseMove(vtkObject* object, unsigned long eid, void* clientdata, void *calldata){

    //Get Event Position
    int* pos = static_cast<vtkInteractorStyle*>(object)->GetInteractor()->GetEventPosition();

    //Pick in 3D
    picker->Pick(pos[0], pos[1], 0.0, ren);
    double* picked = picker->GetPickPosition();

    sphere->SetPosition(picked);

    float uniformPicked[3] = {float(picked[0]), float(picked[1]), float(picked[2])};


    currentActor->GetShaderProperty()->GetVertexCustomUniforms()->SetUniform3f("pickedUniform", uniformPicked);

    renWin->Render();


    static_cast<vtkInteractorStyle*>(object)->OnMouseMove();
}



vtkSmartPointer<vtkActor> MakeActor(vtkSmartPointer<vtkPolyData> polydata){

    vtkSmartPointer<vtkOpenGLPolyDataMapper> mapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
    mapper->SetInputData(polydata);
    mapper->SetVBOShiftScaleMethod(0);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();    
    vtkSmartPointer<vtkShaderProperty> shaderProp = actor->GetShaderProperty();
    actor->SetMapper(mapper);
    return actor;
}

int main(){

    //Read Shader Codes
    std::ifstream vertexShaderFile("../vertex.glsl");
    vertexShaderText = std::string((std::istreambuf_iterator<char>(vertexShaderFile)),std::istreambuf_iterator<char>());

    std::ifstream fragmentShaderFile("../fragment.glsl");
    fragmentShaderText = std::string((std::istreambuf_iterator<char>(fragmentShaderFile)),std::istreambuf_iterator<char>());

    //Initialize RenderWindow and Rendereres
    iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    iren->SetInteractorStyle(interactorStyle);

    //Add Observer
    vtkSmartPointer<vtkCallbackCommand> mouseMoveCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    mouseMoveCallback->SetCallback(onMouseMove);
    interactorStyle->AddObserver(vtkCommand::MouseMoveEvent, mouseMoveCallback );

    renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->SetSize(1000, 1000);
    iren->SetRenderWindow(renWin);
    ren = vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(ren);


    //Initialize Pickewr
    picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.000001);
    picker->PickFromListOn();


    //Initialize Sphere Actor
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();
    sphere = MakeActor(sphereSource->GetOutput());
    sphere->GetProperty()->SetColor(0.0, 0.0, 1.0);
    ren->AddActor(sphere);


    // //read stl
    vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName("../bun_zipper.ply");
    reader->Update();
    vtkSmartPointer<vtkPolyData> polydata = reader->GetOutput();

    //Calculate normals
    vtkSmartPointer<vtkPolyDataNormals> normalCalc = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalCalc->SetInputData(polydata);
    normalCalc->NonManifoldTraversalOn();
    normalCalc->SplittingOff();
    normalCalc->ConsistencyOff();
    normalCalc->ComputePointNormalsOn();
    normalCalc->ComputeCellNormalsOn();
    normalCalc->Update();
    polydata = normalCalc->GetOutput();

    //Sclae x 500
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Scale(500, 500, 500);
    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputData(polydata);
    transformFilter->SetTransform(transform);
    transformFilter->Update();
    polydata = transformFilter->GetOutput();

    //Make Actor
    currentActor = MakeActor(polydata);
    picker->AddPickList(currentActor);

    //Set Shader
    vtkSmartPointer<vtkShaderProperty> shaderProperty = currentActor->GetShaderProperty();
    shaderProperty->SetVertexShaderCode(vertexShaderText.c_str());
    shaderProperty->SetFragmentShaderCode(fragmentShaderText.c_str());
    float uniformValue[3] = {0.0, 0.0, 0.0};
    shaderProperty->GetVertexCustomUniforms()->SetUniform3f("pickedUniform", uniformValue);
    currentActor->GetProperty()->SetColor(0.5, 0.5, 0.0);
    ren->AddActor(currentActor);
    

    renWin->Render();
    iren->Initialize();
    iren->Start();



    std::cout << "Hello" << std::endl;


    return 0;
}