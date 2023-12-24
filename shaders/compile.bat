echo "Compiling vert shaders"
for %%f in (*.vert) do (
    echo %%~f
	c:/VulkanSDK/1.3.268.0/Bin/glslc %%f -o %%f.spv
)

echo "Compiling frag shaders"
for %%f in (*.frag) do (
    echo %%~f
	c:/VulkanSDK/1.3.268.0/Bin/glslc %%f -o %%f.spv
)