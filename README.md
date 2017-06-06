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
  - Existem atualmente 3 tipos de malhas que podem ser geradas, são elas:
  - -> 1) Fibra simples com uma bifurcação no final;
  - -> 2) Fibra com crescimento iterativo;
  - -> 3) Fibra com crescimento iterativo e com o controle do ângulo da bifurcação 
  - Para usar basta passar como argumento para o programa o tamanho de cada fibra, o número de fibras que saiem da cada bifurcação, o tipo da malha e definir o nome para o arquivo de saída.

```sh
$ make
$ ./skeletonMesh <xMax> <biff> <type> <out_VTK_file>
```

  - Pode-se utilizar também o shell script generateSkeleton.sh e definir os parâmetros neste arquivo.
  - Dessa forma o script pode gerar um conjunto de malhas indo de [MIN_MESH,MAX_MESH] para cada tamanho de fibra dentro do intervalo [MIN_SIZE,MAX_SIZE].  

# Mesh_Generator

  - Constrói a malha de elementos finitos a partir da malha vinda do Skeleton_Mesh.
  - Para usar basta passar como argumento para o programa o arquivo gerado pelo Skeleton_Mesh e definir o nome para o arquivo de saída, lembrando que o mesmo deve estar com a extensão '.msh'.
  - Atualmente o programa considera o tamanho de uma célula de Purkinje com um valor de DX=0.006666666666666667 cm (~= 66.67 um).
  - Também existe a opção de se utilizar o shell script 'generateMeshes.sh' para gerar um conjunto de malhas indo de [MIN_MESH,MAX_MESH]. 

```sh
$ make
$ ./meshGenerator <in_VTK_file> <out_MSH_file>
```

# SteadyStateCalculator

  - Constrói um arquivo contendo a solução estacionária para uma determinada malha gerada a partir do Mesh_Generator.
  - O uso é feito a partir da passagem do intervalo de discretização no tempo, dt, o tempo máximo de simulação, o arquivo .msh da malha e um identificador da malha.
  - Pode-se passar o argumento opcional [steady_state_file], com isso será utilizado a solução estacionária de uma simulação já previamente calculada como condição inicial. A única restrição é que deve ser passado um arquivo de uma simulação anterior, por exemplo se for rodar a simulação 31 deve-se passar o arquivo 'steadystate30.dat' como argumento (só funciona com malhas do tipo 1). 

```sh
$ make
$ ./steadyState <dt> <t_max> <mesh_file> <mesh_id> [steady_state_file]
```

  - Existe também um script chamado 'runSimulation.sh' que calcula o estado estacionário das malhas contidas em um diretório qualquer.
  - Os resultados são gravados na pasta definida na variável deste shell script.

```sh
$ ./runSimulation.sh
```

# Solver

  - Resolve a equação monodomínio utilizando o MVF para geração do potencial de ação dos miócitos da fibra.
  - A EDP associada do problema é resolvida usando decomposição LU com pivoteamento. A cada passo de tempo se faz retro+pos substituições.
  - A sistema não linear de EDOs associado é resolvido usando Euler Explícito.
  - Atualmente está versão está configurada para o modelo celular de Noble.
  - Para usar basta passar como argumento para o programa o passo de tempo 'dt', o período máximo da simulação 't_max', o arquivo da malha gerado a partir do Mesh_Generator e o arquivo contendo a solução estacionária vinda do StadyStateCalculator. 
  - Solução fica armazena na pasta VTK contendo os valores do potencial transmembrânico de todos os miócitos.
  - Para visualizar a simulação abrir os arquivos da pasta VTK no Paraview.

```sh
$ make
$ ./purkinjeFEM <dt> <t_max> <mesh_file> <steady_state_file>
```

  - Pode-se executar o script 'runSimulation.sh' para já calcular a solução de um intervalo de malhas contidos em um diretório qualquer.
  - Os resultados ficam armazenados na pasta definida no shell script.

```sh
$ ./runSimulation.sh
```
