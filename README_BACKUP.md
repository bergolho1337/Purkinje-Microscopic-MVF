

# Sst-Neumann

  - Constrói um arquivo contendo a solução estacionária para uma determinada malha gerada a partir do Mesh_Generator.
  - Nesta versão considera-se que nas folhas da árvore se aplica uma condição de contorno do tipo Neumann homogênea. 
  - O uso é feito a partir da passagem do intervalo de discretização no tempo, dt, o tempo máximo de simulação, o arquivo .msh da malha e o nome do arquivo do estado estacionário de saída.

```sh
$ make
$ ./steadystateMVF <dt> <t_max> <mesh_file> <out_steady_state_file>
```

# Solver-Neumann

  - Resolve a equação monodomínio utilizando o MVF para geração do potencial de ação dos miócitos da fibra.
  - A EDP associada do problema é resolvida usando decomposição LU com pivoteamento. A cada passo de tempo se faz retro+pos substituições.
  - A sistema não linear de EDOs associado é resolvido usando Euler Explícito.
  - Atualmente está versão está configurada para o modelo celular de Noble.
  - Para usar basta passar como argumento para o programa o passo de tempo 'dt', o período máximo da simulação 't_max', o arquivo da malha gerado a partir do Mesh_Generator, o arquivo contendo a solução estacionária vinda do Sst-Neumann e definir o número e quais volumes de controle serão analisados para o cálculo da velocidade propagação.
  - A solução fica armazena na pasta VTK contendo os valores do potencial transmembrânico de todos os miócitos.
  - Para visualizar a simulação abrir os arquivos da pasta VTK no Paraview.
  - Informações adicionais a respeito da velocidade de propagação ficam armazenadas na pasta Output.

```sh
$ make
$ ./purkinjeMVF <dt> <t_max> <in_mesh_file> <in_steady_state_file>
```

  - É possível gerar as figuras do potencial de ação de cada volume de controle utilizado para o cálculo da velocidade de propagação a partir do comando 'plot' configurado no Makefile.

```sh
$ make plot
```

  - Também pode-se mover os resultados (figuras + dados) para uma pasta Results. O usuário só precisa especificar o nome do experimento (<out_result_folder>).
  - Os arquivos ficarão salvos em Results/<out_result_folder>.

```sh
$ ./moveResults.sh <out_result_folder>"
```

s