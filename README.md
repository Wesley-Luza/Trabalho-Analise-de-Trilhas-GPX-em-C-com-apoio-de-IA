# Trabalho-Analise-de-Trilhas-GPX-em-C-com-apoio-de-IA
Este trabalho foi realizado na matéria de algoritmos II, do curso de Analise e Desenvolvimento de Sistemas. Trata-se de calcular distância entre coordenadas e ganho/perda de elevação, de Vila Flores/RS a Antônio Prado/RS

## O que o programa faz:

O objetivo é **ler um arquivo GPX**, extrair **latitude**, **longitude** e **elevação** de cada ponto da trilha e calcular:

- Distância total percorrida  
- Ganho de elevação acumulado  
- Perda de elevação acumulada  

O programa foi construído **sem bibliotecas externas**, apenas com tratamento de strings, seguindo todas as restrições do enunciado.

## 📂 Arquivos do Repositório
├── atividadesTrilhasGPX.c # Código completo da solução

├── A.gpx # Arquivo de trilha utilizado nos testes

├── resultado_trilha.exe # Executavel do programa

└── README.md # Este arquivo

## 📌 Funcionalidades Implementadas

### Parsing manual de XML (via `strstr`, `sscanf`, etc.)
O programa encontra e extrai elementos:

```xml
<trkpt lat="..." lon="...">
    <ele>...</ele>
</trkpt>
```

## ✔️ Cálculo de distância — Fórmula de Haversine

Conforme exigido no trabalho:

- Raio da Terra fixado em R = 6371000 metros
- Conversão de graus → radianos
- Distância final acumulada em km com 3 casas decimais

## ✔️ Cálculo de ganho e perda de elevação

Para cada par de pontos:

```
Δele = ele[i+1] - ele[i]
Se Δele > 0  → acumula ganho  
Se Δele < 0  → acumula perda (em valor absoluto)
```
Resultado exibido em *m com 2 casas decimais*.

## 🧠 Decisões de Projeto

As escolhas abaixo foram tomadas durante o desenvolvimento do programa, seguindo os requisitos da prova:

- **Parsing via tratamento de strings:**  
  Atende ao requisito de não usar bibliotecas externas (como libxml2).  
  O XML é lido linha por linha usando `strstr` e `sscanf`.

- **Struct `TrkPt` criada para organização dos dados:**  
  Cada ponto possui `lat`, `lon` e `ele`, facilitando as operações posteriores.

- **Cálculo de distância com fórmula de Haversine:**  
  Implementada manualmente com funções trigonométricas da `math.h`, garantindo precisão adequada.  
  Raio da Terra fixo em **6371000 m** conforme exigido.

- **Organização Modular:**  
  O código foi dividido em funções independentes:  
  - `toRadians()`  
  - `calcularDistanciaHaversine()`  
  - `lerArquivoGPX()`  
  - `processarTrilha()`  
  Esta separação melhora a clareza e manutenção.

- **Array estático de pontos (`MAX_PONTOS`)**  
  Como o tamanho máximo da trilha não é crítico para a prova, foi escolhido um valor seguro e simples: **10000 pontos**.

- **Relatório final formatado conforme a especificação:**  
  - Distância em **km** com **3 casas decimais**  
  - Ganho e perda em **metros** com **2 casas decimais**

## 🛠️ Como Compilar e Executar

### ✔️ Windows (VSCode + MinGW-w64)

1. Instale o compilador **GCC** (via MinGW-w64 ou MSYS2).
2. Abra o terminal integrado do VSCode na pasta do projeto.
3. Compile o programa com:

```bash
gcc atividadesTrilhasGPX.c -o trilha.exe -lm
```
4. Execute:
```
./trilha.exe
```

## Saida Do Programa
<img width="262" height="122" alt="image" src="https://github.com/user-attachments/assets/249e45a7-bbfe-43d5-9058-c1c19f763695" />
