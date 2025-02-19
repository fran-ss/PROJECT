# Projeto Final - Sistemas Embarcados

## EmbarcaTech - Residência Tecnológica
Este projeto foi desenvolvido como parte do programa de capacitação **EmbarcaTech - Residência Tecnológica em Sistemas Embarcados**. O objetivo foi construir um projeto inovador dentro da área de Sistemas Embarcados, aplicando conceitos de Internet das Coisas (IoT). O EmbarcaTech visa capacitar profissionais em tecnologias de Sistemas Embarcados, aliando teoria e prática para impulsionar a inovação e suprir a alta demanda do mercado.

## 📌 Projeto: Servidor HTTP com Raspberry Pi Pico

### 📖 Descrição
Este projeto implementa um **servidor HTTP** utilizando o **Raspberry Pi Pico W** com conectividade Wi-Fi (**CYW43**). O sistema permite o envio de alertas por meio de dois botões, que são exibidos em uma página web acessível via rede local.

### 🔹 Funcionalidades
✅ Conexão Wi-Fi com autenticação WPA2.  
✅ Servidor HTTP rodando na porta 80.  
✅ Registro e exibição de alertas em uma página web.  
✅ Indicação luminosa através de um LED RGB.  

### 🛠 Hardware Utilizado
- **Raspberry Pi Pico W**
- **Placa BitDog Lab**
- **Botões para acionamento de alertas:**
  - **Botão Moderado**: GPIO5
  - **Botão Grave**: GPIO6
- **LED RGB para indicação visual:**
  - **Vermelho**: GPIO13
  - **Verde**: GPIO11
  - **Azul**: GPIO12

## 📌 Configuração do Ambiente
### 🔧 Instalação de Dependências
1. Instale o **SDK do Raspberry Pi Pico**.
2. Instale as bibliotecas necessárias, incluindo `lwIP` e `pico/cyw43_arch`.
3. Configure os dados de Wi-Fi diretamente no código-fonte:
   ```c
   #define WIFI_SSID "sua-rede"
   #define WIFI_PASS "sua-senha"
   ```

### 🏗 Como Compilar e Executar
#### 1️⃣ Configuração do VS Code
- Instale a extensão **Raspberry Pi Pico**.
- Crie um novo projeto e selecione **Pico W** com **Background lwIP**.
- Cole o código principal no arquivo `.c` do projeto.

#### 2️⃣ Criando o `CMakeLists.txt`
Crie um arquivo `CMakeLists.txt` no diretório do projeto com o seguinte conteúdo:
```cmake
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configuração do SDK do Pico
include(pico_sdk_import.cmake)
project(project C CXX ASM)
pico_sdk_init()

# Adiciona o código principal
add_executable(project project.c )
pico_set_program_name(project "project")
pico_set_program_version(project "0.1")

# Configuração de saída via USB
pico_enable_stdio_uart(project 0)
pico_enable_stdio_usb(project 1)

# Vinculação de bibliotecas
 target_link_libraries(project
        pico_stdlib    
        pico_cyw43_arch_lwip_threadsafe_background
        )

pico_add_extra_outputs(project)
```
> **Nota:** Substitua `project` pelo nome do seu arquivo `.c`.

#### 3️⃣ Compilando o Código
- No **VS Code**, abra a extensão **Raspberry Pi Pico** e compile o projeto.
- Verifique no terminal se a compilação ocorreu sem erros.
- O arquivo `.uf2` será gerado.

#### 4️⃣ Enviando o Binário
- Conecte o **Raspberry Pi Pico** ao computador.
- Arraste o arquivo `.uf2` para a unidade correspondente ao Raspberry Pi Pico.
- Conecte-se à rede Wi-Fi configurada e acesse a página pelo navegador digitando o endereço IP do dispositivo.

## 📌 Estrutura do Código
- **`main()`**: Inicializa o Wi-Fi, configura os pinos e inicia o loop principal.
- **`start_http_server()`**: Configura e inicia o servidor HTTP.
- **`http_callback()`**: Processa as requisições HTTP recebidas.
- **`generate_http_response()`**: Gera a resposta HTML com a lista de alertas.
- **`add_alert()`**: Adiciona um alerta à lista armazenada.
- **`set_led_rgb()`**: Controla o LED RGB.

## 📜 Licença
Este projeto é licenciado sob a **MIT License**. Veja o arquivo **LICENSE** para mais detalhes.

