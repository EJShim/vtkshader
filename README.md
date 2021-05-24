#VTK Shader 테스트

## Custom Shader 사용하기

actor 에서 shader 코드를 string 으로 넣어주면 작동
```
shaderProperty = actor.GetShaderProperty()
shaderProperty.SetVertexShaderCode(vertexShaderText)
shaderProperty.SetFragmentShaderCode(fragmentShaderText)
```

## Custom Uniform Variable 사용하기
vtkUniforms 사용, shaderProperty 에서 접근 가능


```
shaderProperty.GetVertexCustomUniforms().SetUniform3f("pickedUniform", [0, 0, 0])
```

```


uniform vec3 pickedUniform;
void main()
{
  //VTK::Color::Impl
    .
    .
    .
```