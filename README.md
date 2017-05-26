# Purkinje-Microscopic-MVF

Resolve a equação do monodomínio utilizando o Método dos Volumes Finitos. O projeto está dividido em 4 partes.

  - Skeleton_Mesh
  - Mesh_Generator
  - SteadyStateCalculator
  - Solver

# Skeleton_Mesh

  - Constrói o esqueleto da malha a ser resolvida.
  - Atualmente está voltada para as simulações ligadas ao estudo do problema de Fonte-Sumidouro nas bifurcações das fibras.
  - A malha de saída do programa está configurada com a extensão .vtk para poder ser visualizada no Paraview. 
  - Para usar basta passar como argumento para o programa o número de bifurcações e o tamanho de cada fibra e definir o nome para o arquivo de saída.

```sh
$ make
$ ./skeletonMesh <xMax> <num_bif> <out_VTK_file>
```

  - Pode-se utilizar também o shell script generateSkeleton.sh e definir o tamanho mínimo e máximo de cada fibra e o número mínimo e máximo de malhas.
  - Dessa forma o script irá gerar um conjunto de malhas indo de [MIN_MESH,MAX_MESH] para cada tamanho de fibra dentro do intervalo [MIN_SIZE,MAX_SIZE].  

# Mesh_Generator

  - Constrói a malha de elementos finitos a partir da malha vinda do Skeleton_Mesh.
  - Para usar basta passar como argumento para o programa o número de elementos que cada fibra irá possuir e o tamanho de cada fibra (deve ser o mesmo que o usado no Skeleton_Mesh).
  - No caso do MVF deve-se escolher o número de elementos finitos que resulte no tamanho da discretização espacial. Sendo dado pela fórmula abaixo: DX = SIZE_FIBER / nElem.
  - Também existe a opção de se utilizar o shell script 'generateMeshes.sh' para gerar um conjunto de malhas indo de [MIN_MESH,MAX_MESH]. Para isso deve-se definir o número de elementos e o tamanho de cada fibra. 

```sh
$ make
$ ./meshGenerator <xMax> <nElem> <in_VTK_file> <out_MSH_file>
```

# SteadyStateCalculator

  - Constrói um arquivo contendo a solução estacionária para uma determinada malha gerada a partir do Mesh_Generator.
  - O uso é feito a partir da passagem do intervalo de discretização no tempo, dt, o tempo máximo de simulação, o arquivo .msh da malha e um identificador da malha.
  - Pode-se passar o argumento opcional [steady_state_file], com isso será utilizado a solução estacionária de uma simulação já previamente calculada como condição inicial. A única restrição é que deve ser passado um arquivo de uma simulação anterior, por exemplo se for rodar a simulação 31 deve-se passar o arquivo 'steadystate30.dat' como argumento. 

```sh
$ make
$ ./steadyState <dt> <t_max> <mesh_file> <mesh_id> [steady_state_file]
```

  - Existe também um script chamado 'runSimulation.sh' que calcula o estado estacionário das malhas contidas no diretório /Malhas.
  - Os resultados são gravados na pasta SteadyState.

```sh
$ ./runSimulation.sh
```

# Solver

  - Resolve a equação monodomínio utilizando o MVF para geração do potencial de ação dos miócitos da fibra.
  - A EDP associada do problema é resolvida usando decomposição LU com pivoteamento. A cada passo de tempo se faz retro+pos substituições.
  - A sistema não linear de EDOs associado é resolvido usando Euler Explícito.
  - Atualmente está versão está configurada para o modelo celular de Noble.
  - Para usar basta passar como argumento para o programa o passo de tempo 'dt', o período máximo da simulação 't_max', o arquivo da malha gerado a partir do Mesh_Generator e o arquivo contendo a solução estacionária vinda do StadyStateCalculator. 
  - Solução fica armazena na pasta VTK contendo os valores do potencial transmembrânico e da corrente de todos os miócitos.
  - Para visualizar a simulação abrir os arquivos da pasta VTK no Paraview.

```sh
$ make
$ ./purkinjeFEM <dt> <t_max> <mesh_file> <steady_state_file>
```

  - Pode-se executar o script 'runSimulation.sh' para já calcular a solução de um intervalo de malhas contidos no diretório /Malhas.
  - Os resultados ficam armazenados na pasta /Resultados.

```sh
$ ./runSimulation.sh
```
