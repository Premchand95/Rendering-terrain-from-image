#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkPNGReader.h>
#include <vtkImageMagnitude.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkWarpScalar.h>
#include <vtkMergeFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

int main(int argc, char* argv[])
{
  //Verify input arguments
  if ( argc != 2 )
  {
    std::cout << "Usage: " << argv[0]
              << " Filename(.pnm)" << std::endl;
    return EXIT_FAILURE;
  }

  //Read the image
  vtkSmartPointer<vtkPNGReader> reader =
    vtkSmartPointer<vtkPNGReader>::New();
  reader->SetFileName ( argv[1] );

  //use vtkimagemagnitude
  reader->SetDataScalarTypeToUnsignedChar();
  vtkSmartPointer< vtkImageMagnitude > magnitude =
	  vtkSmartPointer< vtkImageMagnitude >::New();
  magnitude->SetInputConnection(reader->GetOutputPort());
  magnitude->Update();
  //use vtkgeometry
  // Convert the image intensities to a polydata
  vtkSmartPointer<vtkImageDataGeometryFilter> imageDataGeometryFilter =
	  vtkSmartPointer<vtkImageDataGeometryFilter>::New();
  imageDataGeometryFilter->SetInputConnection(magnitude->GetOutputPort());
  imageDataGeometryFilter->Update();
  //wrapping the geometry
  vtkSmartPointer<vtkWarpScalar> warpScalar =
	  vtkSmartPointer<vtkWarpScalar>::New();
  warpScalar->SetInputConnection(imageDataGeometryFilter->GetOutputPort());
  warpScalar->SetScaleFactor(0.2); // use the scalars themselves

  // Required for SetNormal to have an effect
  warpScalar->UseNormalOn();
  warpScalar->SetNormal(0, 0, 1);

  warpScalar->Update();

  //merge geometry and image
  vtkSmartPointer<vtkMergeFilter> merge =
	  vtkSmartPointer<vtkMergeFilter>::New();
  merge->SetGeometryConnection(warpScalar->GetOutputPort());
  merge->SetScalarsConnection(reader->GetOutputPort());
  // Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper =
	  vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(merge->GetOutputPort());
  mapper->SetScalarRange(0, 255);

  vtkSmartPointer<vtkActor> actor =
	  vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  // Visualization
  vtkSmartPointer<vtkRenderer> renderer =
	  vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
	  vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
	  vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(1, 1, 1); // Background color white
  renderWindow->SetSize(512,512);

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
