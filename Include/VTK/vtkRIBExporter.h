/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkRIBExporter.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkRIBExporter - export a scene into RenderMan RIB format.
// .SECTION Description
// vtkRIBExporter is a concrete subclass of vtkExporter that writes a
// Renderman .RIB files. The input specifies a vtkRenderWindow. All
// visible actors and lights will be included in the rib file. The
// following file naming conventions apply:
//   rib file - FilePrefix.rib
//   image file created by RenderMan - FilePrefix.tif
//   texture files - TexturePrefix_0xADDR_MTIME.tif
// This object does NOT generate an image file. The user must run either
// RenderMan or a RenderMan emulator like Blue Moon Ray Tracer (BMRT).
// vtk properties are convert to Renderman shaders as follows:
//   Normal property, no texture map - plastic.sl
//   Normal property with texture map - txtplastic.sl
// These two shaders must be compiled by the rendering package being
// used.  vtkRIBExporter also supports custom shaders. The shaders are
// written using the Renderman Shading Language. See "The Renderman
// Companion", ISBN 0-201-50868, 1989 for details on writing shaders.
// vtkRIBProperty specifies the declarations and parameter settings for
// custom shaders.
// Tcl Example: generate a rib file for the current rendering.
// vtkRIBExporter myRIB
//   myRIB SetInput $renWin
//   myRIB SetFIlePrefix mine
//   myRIB Write
// This will create a file mine.rib. After running this file through
// a Renderman renderer a file mine.tif will contain the rendered image.
//
// .SECTION See Also
// vtkExporter vtkRIBProperty


#ifndef __vtkRIBExporter_h
#define __vtkRIBExporter_h

#include "vtkExporter.h"

class vtkActor;
class vtkCamera;
class vtkLight;
class vtkPolyData;
class vtkProperty;
class vtkRenderer;
class vtkTexture;
class vtkUnsignedCharArray;

class VTK_HYBRID_EXPORT vtkRIBExporter : public vtkExporter
{
public:
  static vtkRIBExporter *New();
  vtkTypeRevisionMacro(vtkRIBExporter,vtkExporter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description
  // Specify the size of the image for RenderMan. If none is specified, the
  // size of the render window will be used.
  vtkSetVector2Macro(Size,int);
  vtkGetVectorMacro(Size,int,2);

  // Description
  // Specify the sampling rate for the rendering. Default is 2 2.
  vtkSetVector2Macro(PixelSamples,int);
  vtkGetVectorMacro(PixelSamples,int,2);

  // Description:
  // Specify the prefix of the files to write out. The resulting file names
  // will have .RIB appended to them.
  vtkSetStringMacro(FilePrefix);
  vtkGetStringMacro(FilePrefix);

  // Description:
  // Specify the prefix of any generated texture files.
  vtkSetStringMacro(TexturePrefix);
  vtkGetStringMacro(TexturePrefix);

  // Description:
  // Set/Get the background flag. Default is 0 (off).
  // If set, the rib file will contain an
  // image shader that will use the renderer window's background
  // color. Normally, RenderMan does generate backgrounds. Backgrounds are
  // composited into the scene with the tiffcomp program that comes with
  // Pixar's RenderMan Toolkit.  In fact, Pixar's Renderman will accept an
  // image shader but only sets the alpha of the background. Images created
  // this way will still have a black background but contain an alpha of 1
  // at all pixels and CANNOT be subsequently composited with other images
  // using tiffcomp.  However, other RenderMan compliant renderers like
  // Blue Moon Ray Tracing (BMRT) do allow image shaders and properly set
  // the background color. If this sounds too confusing, use the following
  // rules: If you are using Pixar's Renderman, leave the Background
  // off. Otherwise, try setting BackGroundOn and see if you get the
  // desired results.
  vtkSetMacro(Background,int);
  vtkGetMacro(Background,int);
  vtkBooleanMacro(Background,int);

  // Description:
  // Set or get the ExportArrays. If ExportArrays is set, then
  // all point data, field data, and cell data arrays will get 
  // exported together with polygons.
  vtkSetClampMacro(ExportArrays, int, 0, 1);
  vtkBooleanMacro(ExportArrays, int);
  vtkGetMacro(ExportArrays, int);

protected:
  vtkRIBExporter();
  ~vtkRIBExporter();

  int Background;
  int Size[2];
  int PixelSamples[2];

  // Description:
  // This variable defines whether the arrays are exported or not.
  int ExportArrays;

  // Description:
  // Write the RIB header.
  void WriteHeader (vtkRenderer *aRen);
  void WriteTrailer ();
  void WriteTexture (vtkTexture *aTexture);
  void WriteViewport (vtkRenderer *aRenderer, int size[2]);
  void WriteCamera (vtkCamera *aCamera);
  void WriteLight (vtkLight *aLight, int count);
  void WriteAmbientLight (int count);
  void WriteProperty (vtkProperty *aProperty, vtkTexture *aTexture);
  void WritePolygons (vtkPolyData *pd, vtkUnsignedCharArray *colors, 
                      vtkProperty *aProperty);
  void WriteStrips (vtkPolyData *pd, vtkUnsignedCharArray *colors, 
                    vtkProperty *aProperty);

  void WriteData();
  void WriteActor(vtkActor *anActor);
  
  // Description:
  // Since additional variables are sent to the shader as
  // variables, and their names are used in the shader, these
  // names have to follow C naming convention. This method
  // modifies array name so that you can use it in shader.
  void ModifyArrayName(char *newname, const char* name);

  char *GetTextureName (vtkTexture *aTexture);
  char *GetTIFFName (vtkTexture *aTexture);
  char *FilePrefix;
  FILE *FilePtr;
  char *TexturePrefix;
private:
  vtkRIBExporter(const vtkRIBExporter&);  // Not implemented.
  void operator=(const vtkRIBExporter&);  // Not implemented.
};

#endif

