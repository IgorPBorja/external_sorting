# Arquivos

## `beta.txt`

Foram gerados $10$ vetores $v_1, \dotsc, v_10$ aleatórios de tamanho $10^6$
e com elementos amostrados de uma distribuição aproximadamente uniforme em
$[-10^9, 10^9]$ (foi utilizada o valor da função `rand()` em C++, módulo o tamanho do intervalo
somada ao elemento mínimo).

Para cada $m \in \{5k: 1 \leq k \leq 20\}$ foi medida a métrica
$\beta(m, 0)$ para o conjunto de dados $v_i$, denotada aqui por
$\beta(m, 0, i)$, e calculamos a sua média:
$$\hat{\beta}(m, 0) = \dfrac{1}{10}\sum_{1 \leq i \leq 10}\beta(m, 0, i)$$
e seu desvio padrão $\sigma(m, 0)$.
