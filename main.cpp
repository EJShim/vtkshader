#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkPolyData.h>
#include <vtkOpenGLActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkAutoInit.h>
#include <vtkShaderProperty.h>
#include <vtkUniforms.h>
#include <vtkMatrix4x4.h>
#include <vtkMatrix3x3.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

vtkSmartPointer<vtkRenderWindowInteractor> iren;
vtkSmartPointer<vtkRenderWindow> renWin;
vtkSmartPointer<vtkRenderer> ren;
vtkSmartPointer<vtkActor> currentActor;
vtkSmartPointer<vtkActor> MakeActor(vtkSmartPointer<vtkPolyData> polydata){

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polydata);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkShaderProperty> shaderProp = actor->GetShaderProperty();
    

    vtkMatrix4x4* mcwc;
    vtkMatrix3x3* anorms;
    static_cast<vtkOpenGLActor*>(actor.GetPointer())->GetKeyMatrices(mcwc, anorms);

    vtkIndent indent;
    mcwc->PrintSelf(std::cout, indent);



    actor->SetMapper(mapper);
    return actor;
}

int main(){

    iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    iren->SetInteractorStyle(interactorStyle);
    renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->SetSize(1000, 1000);
    iren->SetRenderWindow(renWin);

    ren = vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(ren);




    // //read stl
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName("../mn.stl");
    reader->Update();
    vtkSmartPointer<vtkPolyData> polydata = reader->GetOutput();
    currentActor = MakeActor(polydata);

    ren->AddActor(currentActor);
    






    renWin->Render();
    iren->Initialize();
    iren->Start();





    return 0;
}