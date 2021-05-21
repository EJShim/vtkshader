import vtk
import os
import random
vertexShaderText = ""
fragmentShaderText = ""

iren = vtk.vtkRenderWindowInteractor()
interactorStyle = vtk.vtkInteractorStyleTrackballCamera()

iren.SetInteractorStyle(interactorStyle)

renWin = vtk.vtkRenderWindow()
renWin.SetSize(1000, 1000)
iren.SetRenderWindow(renWin)

ren = vtk.vtkRenderer()
renWin.AddRenderer(ren)

sphereActor = None
actor = None
shaderProperty = None

picker = vtk.vtkCellPicker()
picker.SetTolerance(0.000001)
picker.PickFromListOn()


def onMouseMove(interactor, b):
    

    
    pos = interactor.GetInteractor().GetEventPosition()
    
    
    
    
    picker.Pick(pos[0], pos[1], 0.0, ren)

    picked =  picker.GetPickPosition()
    sphereActor.SetPosition(picked)

    #Add Shader Property
    # light.SetPosition(ren.GetActiveCamera().GetPosition())
    shaderProperty.GetVertexCustomUniforms().SetUniform3f("pickedUniform", [picked[0], picked[1], picked[2]])

    renWin.Render()

    interactor.OnMouseMove()

interactorStyle.AddObserver("MouseMoveEvent", onMouseMove)
#Get Shader Text        
with open("vertex.glsl", "r") as vertexShaderFile:
    vertexShaderText = vertexShaderFile.read()
with open( "fragment.glsl", 'r') as shaderFile:
    fragmentShaderText = shaderFile.read()

def MakeActor(polydata):
    mapper = vtk.vtkOpenGLPolyDataMapper()
    mapper.SetInputData(polydata)

    print(mapper.SetVBOShiftScaleMethod(0))



    actor = vtk.vtkOpenGLActor()
    actor.SetMapper(mapper)
    picker.AddPickList(actor)


    return actor

if __name__ == "__main__":

    sphereSource = vtk.vtkSphereSource()
    sphereSource.Update()
    sphereActor = MakeActor(sphereSource.GetOutput())
    sphereActor.GetProperty().SetColor(1, 0, 0)
    # ren.AddActor(sphereActor)

    #Read Sample Data
    reader = vtk.vtkSTLReader()
    reader.SetFileName("mx.stl")
    reader.Update()

    polydata = reader.GetOutput()


    normalCalc = vtk.vtkPolyDataNormals()
    # normalCalc.SetInputData(result)
    normalCalc.SetInputData(polydata)
    normalCalc.NonManifoldTraversalOn()
    normalCalc.SplittingOff()
    normalCalc.ConsistencyOff()
    normalCalc.ComputePointNormalsOn()
    normalCalc.ComputeCellNormalsOn()
    normalCalc.Update()
    polydata = normalCalc.GetOutput()


    actor = MakeActor(polydata)

    
    shaderProperty = actor.GetShaderProperty()
    shaderProperty.SetVertexShaderCode(vertexShaderText)
    shaderProperty.SetFragmentShaderCode(fragmentShaderText)
    shaderProperty.GetVertexCustomUniforms().SetUniform3f("pickedUniform", [0, 0, 0])

    # shaderProperty.GetFragmentCustomUniforms().SetUniform3f("pickedUniform", [1.0, 0.0, 0.0])

    ren.AddActor(actor)
    
    # ren.AddLight(light)
    renWin.Render()

    iren.Initialize()
    iren.Start()