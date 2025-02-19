# Projeto Final - Sistemas Embarcados

##Este projeto foi desenvolvido como parte do programa de capacitação "EmbarcaTech - Residência Tecnológica em Sistemas Embarcados". O objetivo foi construir um projeto inovador dentro da área de Sistemas Embarcados, aplicando conceitos de Internet das Coisas (IoT). O EmbarcaTech visa capacitar profissionais em tecnologias de Sistemas Embarcados, aliando teoria e prática para impulsionar a inovação e suprir a alta demanda do mercado.

# Projeto: Servidor HTTP com Raspberry Pi Pico

## Descrição
Este projeto implementa um servidor HTTP utilizando o microcontrolador Raspberry Pi Pico com conectividade Wi-Fi (CYW43). O sistema permite o envio de alertas por meio de dois botões, que são exibidos em uma página web acessível via rede local.

## Funcionalidades
- Conexão Wi-Fi com autenticação WPA2.
- Servidor HTTP rodando na porta 80.
- Registro e exibição de alertas em uma página web.
- Indicação luminosa através de um LED RGB.

## Hardware Utilizado
- Raspberry Pi Pico W
- Placa BitDog Lab
- Botões para acionamento de alertas:
  - **Botão Moderado**: GPIO5
  - **Botão Grave**: GPIO6
- LED RGB para indicação visual:
  - **Vermelho**: GPIO13
  - **Verde**: GPIO11
  - **Azul**: GPIO12

## Configuração do Ambiente
1. Instale o SDK do Raspberry Pi Pico.
2. Instale as bibliotecas necessárias, incluindo `lwIP` e `pico/cyw43_arch`.
3. Configure os dados de Wi-Fi diretamente no código-fonte:
   ```c
   #define WIFI_SSID "sua-rede"
   #define WIFI_PASS "sua-senha"
   ```

## Como Compilar e Executar
### 1. Configuração do VS Code
- Instale a extensão **Raspberry Pi Pico**.
- Crie um novo projeto e selecione **Pico W** com **Background lwIP**.
- Cole o código principal no arquivo `.c` do projeto.

### 2. Criando o `CMakeLists.txt`
- No diretório do projeto, crie um arquivo `CMakeLists.txt` com o seguinte conteúdo:
   ```cmake
  # Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(project C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(project project.c )

pico_set_program_name(project "project")
pico_set_program_version(project "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(project 0)
pico_enable_stdio_usb(project 1)

# Add the standard library to the build
target_link_libraries(project
        pico_stdlib    
        pico_cyw43_arch_lwip_threadsafe_background
        )
        
# Add the standard include files to the build
target_include_directories(project PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(project)

   ```
   > **Nota:** Modifique `project` para o nome do seu arquivo `.c`.

### 3. Compilando o Código
- No VS Code, abra a extensão **Raspberry Pi Pico** e compile o projeto.
- Verifique no terminal se a compilação ocorreu sem erros.
- O arquivo `.uf2` será gerado.

### 4. Enviando o Binário
- Arraste o arquivo `.uf2` para a unidade correspondente ao Raspberry Pi Pico.
- Conecte-se à rede Wi-Fi configurada e acesse a página pelo navegador digitando o endereço IP do dispositivo.

## Estrutura do Código
- `main()`: Inicializa o Wi-Fi, configura os pinos e inicia o loop principal.
- `start_http_server()`: Configura e inicia o servidor HTTP.
- `http_callback()`: Processa as requisições HTTP recebidas.
- `generate_http_response()`: Gera a resposta HTML com a lista de alertas.
- `add_alert()`: Adiciona um alerta à lista armazenada.
- `set_led_rgb()`: Controla o LED RGB.

## Licença
Este projeto é licenciado sob a MIT License. Veja o arquivo LICENSE para mais detalhes.

