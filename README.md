# Purkinje-Microscopic-MVF

Solve the monodomain equation using Finite Volume Method. 

![Monodomain equation](img/monodomain.jpg)

The project is divided in 4 parts.

- Skeleton_Mesh
- Mesh_Generator
- Pk-Noble
- Pk-LiRudy

# Skeleton_Mesh

- Build the skeleton for the network mesh.
- The output file is using the .vtk extension.
- To visualize the structure open the output file using Paraview
- There are 3 types of network that could be generated:
  - Fiber with a bifurcation at the endpoints (Type 1);
  - Fiber with iterative growth (Type 2);
  - Fiber with iterative growth and angle control (Type 3). 
- For the Type 2 and 3 network the maximum number of iterations and angle control can be adjusted by the variables MAX_ITER and ANG, respectly.

Build and execute.

```sh
$ make
$ ./skeletonMesh <xMax> <biff> <type> <out_VTK_file>
```

Example.
```sh
$ ./skeletonMesh 1.0 2 3 example.vtk
```
This example will build a network of type 3 with two segments leaving each bifurcation and a fiber length of 1cm.

![Generated network](img/net1.png)