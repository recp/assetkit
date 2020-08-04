Quick Implementation
===================================

Assuming you already followed Build instructions and Getting Started sections.
Also assuming you already linked **AssetKit** to your project and set include paths.

1. Include
----------------

**AssetKit** uses **ak** prefix for all functions and type names. To include **AssetKit** 

.. code-block:: c
  :linenos:

  #include <ak/assetkit.h>

  /* other headers */
  #include <ak/options.h>
  ...

2. Preparing
----------------

You may want to prepare loader before call :c:func:`ak_load` load function. 

a. Setting Image loader
~~~~~~~~~~~~~~~~~~~~~~~~

There was image loader inside **AssetKit** but it has been dropped to make it more generic.
Becasue you may already have an image loader e.g. stb_image ... 

**AssetKit** can trigger images and cache them for you, if it is already loaded than it will return loaded contents.

You need to set loader as below. The example used stb_image but you mat use another image loader...

.. code-block:: c
  :linenos:

  void*
  imageLoadFromFile(const char * __restrict path,
                    int        * __restrict width,
                    int        * __restrict height,
                    int        * __restrict components) {
    return stbi_load(path, width, height, components, 0);
  }

  void*
  imageLoadFromMemory(const char * __restrict data,
                      size_t                  len,
                      int        * __restrict width,
                      int        * __restrict height,
                      int        * __restrict components) {
    return stbi_load_from_memory((stbi_uc const*)data, (int)len, width, height, components, 0);
  }

  void
  imageFlipVerticallyOnLoad(bool flip) {
    stbi_set_flip_vertically_on_load(flip);
  }


  /* Call this before loading document e.g. ak_load() or images */
  ak_imageInitLoader(imageLoadFromFile, imageLoadFromMemory, imageFlipVerticallyOnLoad);

Just ensure that you set image loader before loading images. It is good to set it once before :c:func:`ak_load`.

b. Setting Options if Needed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Make sure that you set UP axis if you want to different one from **Y_UP**. 
For instance if you want to load contents as **Z_UP** 

.. code-block:: c

  ak_opt_set(AK_OPT_COORD, (uintptr_t)AK_ZUP);

see options in the documentation. If the default values don't work for you then just change them before :c:func:`ak_load`.

3. Load Document
----------------

Use :c:func:`ak_load()` to load a 3D file. It returns :c:type:`AkDoc` which contains everything you need.
You must check the result/return, it must be AK_OK otherwise, document is not loaded or falied at some point.

.. code-block:: c
  :linenos:

  AkDoc   *doc;
  AkResult ret;
  
  if ((ret = ak_load(&doc, "[Path to a file e.g ./sample.gltf]", NULL) != AK_OK) {
     printf("Document couldn't be loaded");
     return;
  }

or 

.. code-block:: c
  :linenos:

  AkDoc   *doc;
  AkResult ret;
  
  ret = ak_load(&doc, "[Path to a file e.g ./sample.gltf]", NULL);
  if (ret != AK_OK) {
     printf("Document couldn't be loaded");
     return;
  }

**doc** is passed as reference, if the result is success than the document will be set that reference parameter.

**AssetKit** will try to load referenced textures, images, binary files... so you must only pass original file, not folder.

Now you loaded the document you want. See next step.

------

There are two ways to load geometries from loaded document.

a. Load scene[s], nodes than load referenced geometries
b. Load all geometries in the document

The second way may cause to load unused geometries, because a geometry may not be referenced in scenes.
It is better to follow scene > node > instance geometry > geometry path.

4. Load Scene[s]
----------------

**AssetKit** can load scenes, nodes, geometries and so on. If the file you loaded doesn't support scenes e.g Wavefront Obj.
AssetKit creates a default scene for that file formats and adds reference of geometries to that scene.

There are **scene library** and **scene** in AssetKit **document**. The **scene** is the active scene for rendering, it references a scene from library.

.. code-block:: c
  :linenos:

  AkInstanceBase *instScene;
  AkVisualScene  *scene;

  if ((instScene = doc->scene.visualScene)) {
    scene = (AkVisualScene *)ak_instanceObject(doc->scene.visualScene);
  }

`scene.visualScene` is instance reference ( :c:type:`AkInstanceBase` ), any scene may be instantiated with this link/object.
Another instance objects may have different types e.g. instance geometry (inherited from :c:type:`AkInstanceBase`).

We need to get actual scene object from instance object. There are a few helpers for this task.
But we will use :c:func:`ak_instanceObject` here. 