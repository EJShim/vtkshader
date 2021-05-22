import vtk
import os
import random
vertexShaderText = ""
fragmentShaderText = ""

#Get Shader Text        
with open("vertex.glsl", "r") as vertexShaderFile:
    vertexShaderText = vertexShaderFile.read()
with open( "fragment.glsl", 'r') as shaderFile:
    fragmentShaderText = shaderFile.read()
sphereActor = None
actor = None
shaderProperty = None


#Initialize Renderer
iren = vtk.vtkRenderWindowInteractor()
interactorStyle = vtk.vtkInteractorStyleTrackballCamera()
iren.SetInteractorStyle(interactorStyle)
renWin = vtk.vtkRenderWindow()
renWin.SetSize(1000, 1000)
iren.SetRenderWindow(renWin)
ren = vtk.vtkRenderer()
renWin.AddRenderer(ren)


#Initialize Picker
picker = vtk.vtkCellPicker()
picker.SetTolerance(0.000001)
picker.PickFromListOn()


def onMouseMove(interactor, b):
    #Pick a psoition  
    pos = interactor.GetInteractor().GetEventPosition()
    picker.Pick(pos[0], pos[1], 0.0, ren)
    picked =  picker.GetPickPosition()

    #Update Sphere actor Position
    sphereActor.SetPosition(picked)

    #Update Shader Property    
    shaderProperty.GetVertexCustomUniforms().SetUniform3f("pickedUniform", [picked[0], picked[1], picked[2]])

    #Redraw
    renWin.Render()
    ren.ResetCameraClippingRange()
    interactor.OnMouseMove()

#InteractorStyle Observer
interactorStyle.AddObserver("MouseMoveEvent", onMouseMove)


def MakeActor(polydata):
    mapper = vtk.vtkOpenGLPolyDataMapper()
    mapper.SetInputData(polydata)

    #Disable VBO shfit and scale
    mapper.SetVBOShiftScaleMethod(0)
    actor = vtk.vtkOpenGLActor()
    actor.SetMapper(mapper)


    return actor

if __name__ == "__main__":

    sphereSource = vtk.vtkSphereSource()
    sphereSource.Update()
    sphereActor = MakeActor(sphereSource.GetOutput())
    sphereActor.GetProperty().SetColor(0, 0, 1)
    ren.AddActor(sphereActor)

    #Read Sample Data
    reader = vtk.vtkPLYReader()
    reader.SetFileName("bun_zipper.ply")
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


    transform = vtk.vtkTransform()
    transform.Scale(500, 500, 500)
    transformFilter = vtk.vtkTransformPolyDataFilter()
    transformFilter.SetInputData(polydata)
    transformFilter.SetTransform(transform)
    transformFilter.Update()

    polydata = transformFilter.GetOutput()


    actor = MakeActor(polydata)
    actor.GetProperty().SetColor(0.5, 0.5, 0.0)    
    picker.AddPickList(actor)

    shaderProperty = actor.GetShaderProperty()
    shaderProperty.SetVertexShaderCode(vertexShaderText)
    shaderProperty.SetFragmentShaderCode(fragmentShaderText)
    shaderProperty.GetVertexCustomUniforms().SetUniform3f("pickedUniform", [0, 0, 0])    

    ren.AddActor(actor)    
    
    
    renWin.Render()
    iren.Initialize()
    iren.Start()