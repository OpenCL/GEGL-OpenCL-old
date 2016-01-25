# GEGL-OpenCL
#### First official OPP (OpenCL Porting Project) of OpenCL.org

[GEGL](http://gegl.org/) (Generic Graphics Library) is a graph based image processing framework.

With [GEGL](http://gegl.org/) you chain together image processing operations represented by nodes into a graph. [GEGL](http://gegl.org/) provides such operations for loading and storing images, adjusting colors, filtering in different ways, transforming and compositing images.

[GEGL](http://gegl.org/) also depends on [BABL](http://www.gegl.org/babl/) to translate pixel formats. [BABL](http://www.gegl.org/babl/) allows converting between different methods of storing pixels known as pixel formats that have with different bitdepths and other data representations, color models and component permutations. A vocabulary to formulate new pixel formats from existing primitives is provided as well as the framework to add new color models and data types.

For a brief explanation on how GEGL works, read this [document](https://onedrive.live.com/redir?resid=B05FBF0EF1DAF9A4!7923&authkey=!ALQ1C8VwMFWrtwc&ithint=file%2cdocx)

#### Development

There already exists OpenCL integration into GEGL and some operations have already been ported to run on OpenCL. However, there is still a lot of operations that need porting as listed in the Google Sheet below.

[To-do List for GEGL-OpenCL](https://docs.google.com/spreadsheets/d/1gzgFiw-GVcLcENLLpPP0-LTQd_B4Zc1x4JCi95Girf8/edit?usp=drivesdk)

Likewise, we have a slack channel for discussions pertaining to development and issues here, [GEGL-OpenCL Slack](https://gegl-opencl.slack.com/)





