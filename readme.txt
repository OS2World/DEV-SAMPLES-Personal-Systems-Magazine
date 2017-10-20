VALETD.ZIP contains all of the code necessary to run the DSOM version
of the Valet sample.  To run this sample, and setup an environment for
changing the sample, do the following:

       1. Download VALETD.ZIP in binary format
       2. Create a directory called C:\VALETD\
       3. UNZIP VALETD.ZIP in C:\VALETD\ with the -D option:
               PKUNZIP -D VALETD.ZIP
       4. Inspect ENV.CMD to validate the paths for your environment,
               alter as needed and execute this command file.
       5. START SOMDD.EXE
       6. Run CLIENT.EXE

The sample was built using the SOMOBJECTS 3.0 BETA.

The following files are included for convenience when working with this
sample:

       ENV.CMD configures the environment variables
       CLEAN.CMD deletes the generated files from a directory
       CLEANALL.CMD deletes the generated files from all directories
       GETDLLS.CMD moves DLLs around
       BUILD.CMD rebuilds the sample by deleting files first
       MAKALL.CMD re-makes the sample
