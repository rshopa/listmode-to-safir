## Example of reconstruction for 1 mm source placed at (X, Y, Z) = (10 cm, 0 cm, 0 cm)

#### Usage
```
$ lm_to_projdata lm_to_projdata.par
$ FBP3DRP FBP3DRP.par
$ stir_write_pgm --orientation t --slice_index 63 image_t.pgm reconstructed_image.hv
$ stir_write_pgm --orientation s --slice_index 150 image_s.pgm reconstructed_image.hv
$ stir_write_pgm --orientation c --slice_index 122 image_c.pgm reconstructed_image.hv
```
#### Files description
* **lm_to_projdata.par** - parameters for STIR's **lm_to_projdata** command
* **listmode_input_JPET_SAFIR.par** - parameters for SAFIR
* **7mm_x10y0z0.clm.safir** - MUPET file generated from list mode
* **crystal_map_jpet.txt** - crystal map for SAFIR usage
* **JPET_test_file.hs, JPET_test_file.s** - "empty" sinogram (with scanner parameters in header)
* **1mm_source_f1g1d0b0.hs, 1mm_source_f1g1d0b0.s** - names for generated sinogram (not presented)
* **FBP3DRP.par** - reconstruction parameters
* **reconstructed_image.ahv, reconstructed_image.hv, reconstructed_image.v** - reconstructed image (not present)
* **image_t.pgm, image_s.pgm, image_c.pgm** - images in XY, YZ and XZ cuts, respectively (not present)
