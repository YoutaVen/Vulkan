VulKan ToolS (VKTS) Exporter:
-----------------------------

This folder contains the [Blender](http://www.blender.org) exporter for the VKTS scene file format.

To use the Blender exporter, copy the folder `io_scene` and its content of the `Blender` folder  
into the following subfolder of your Blender installation:

`X.XX/scripts/addons`, where `X.XX` is the Blender version.

After this, the exporter has to be enabled in the Add-ons tab of the user preferences inside Blender.


Exporting can take quite some time depending on the scene. Please wait, even the export seems to be frozen.
At the moment, the Python export script stops, if a reference to an object is not found. In this case,
please remove unreferenced objects, save the scene and close Blender. Now the export should work.
