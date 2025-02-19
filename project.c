#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#define BTN_MODERADO 5     // Botão A - GPIO5
#define BTN_GRAVE 6        // Botão B - GPIO6
#define LED_R 13           // Vermelho - GPIO13
#define LED_G 11           // Verde - GPIO11
#define LED_B 12           // Azul - GPIO12
#define WIFI_SSID "sua-rede"
#define WIFI_PASS "sua-senha"
#define MAX_ALERTS 10

char alert_list[MAX_ALERTS][50];
int alert_count = 0;

// Protótipo da função set_led_rgb
void set_led_rgb(bool r, bool g, bool b);

void add_alert(const char *alert) {
    if (alert_count < MAX_ALERTS) {
        strcpy(alert_list[alert_count], alert);
        alert_count++;
    } else {
        for (int i = 0; i < MAX_ALERTS - 1; i++) {
            strcpy(alert_list[i], alert_list[i + 1]);
        }
        strcpy(alert_list[MAX_ALERTS - 1], alert);
    }
}

void generate_http_response(char *buffer) {
    char alert_list_html[512] = "";
    for (int i = 0; i < alert_count; i++) {
        char temp[100];
        sprintf(temp, "<li>%s</li>", alert_list[i]);
        strcat(alert_list_html, temp);
    }

    sprintf(buffer,
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        "<!DOCTYPE html><html><body>"
        "<h1>Lista de Alertas</h1>"
        "<ul>%s</ul>"
        "<p><a href='/'>Atualizar</a></p>"
        "</body></html>\r\n", alert_list_html);
}

static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    char *request = (char *)p->payload;
    printf("Requisição recebida: %s\n", request);
    
    // Ao receber a requisição, acende o LED verde (indicação de que o pedido foi recebido)
    //set_led_rgb(0, 1, 0); // LED Verde aceso (vermelho=0, verde=1, azul=0)
    
    // Verifica se a requisição é a página inicial
    if (strstr(request, "GET /") || strstr(request, "GET /index.html")) {
        char response[512];
        generate_http_response(response);
        tcp_write(tpcb, response, strlen(response), TCP_WRITE_FLAG_COPY);
    }

    // Desliga o LED verde após o processamento da requisição
    set_led_rgb(0, 0, 0); // Desliga o LED RGB
    pbuf_free(p);
    return ERR_OK;
}

static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_callback);
    return ERR_OK;
}

static void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao ligar o servidor na porta 80\n");
        return;
    }

    pcb = tcp_listen(pcb);
    tcp_accept(pcb, connection_callback);
    printf("Servidor HTTP rodando na porta 80...\n");
}

// Função para controlar o LED RGB
void set_led_rgb(bool r, bool g, bool b) {
    gpio_put(LED_R, r);
    gpio_put(LED_G, g);
    gpio_put(LED_B, b);
}

int main() {
    stdio_init_all();
    sleep_ms(5000);
    printf("Iniciando servidor HTTP\n");

    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
         // Piscar o LED indicando falha na conexão
        for (int i = 0; i < 4; i++) {  // Pisca 5 vezes
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);  // Apaga o LED
            sleep_ms(100);  // Espera 100ms
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);  // Acende o LED
            sleep_ms(100);  // Espera 100ms
        }
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);  // Apaga o LED
        return 1;  // Retorna após o pisca
    }
    
    uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
    printf("Wi-Fi conectado! Endereço IP: %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    
    start_http_server();
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1); // Apaga LEDs

    
    gpio_init(BTN_MODERADO);
    gpio_set_dir(BTN_MODERADO, GPIO_IN);
    gpio_pull_up(BTN_MODERADO);
    
    gpio_init(BTN_GRAVE);
    gpio_set_dir(BTN_GRAVE, GPIO_IN);
    gpio_pull_up(BTN_GRAVE);

    // Configuração do LED RGB
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_put(LED_R, 0);
    
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 0);
    
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, 0);
    
    while (true) {
        cyw43_arch_poll();
        
        if (gpio_get(BTN_MODERADO) == 0) {
            add_alert("Alerta Moderado Recebido!");
            printf("Alerta Moderado enviado!\n");

            set_led_rgb(1, 1, 0); // Amarelo (Vermelho + Verde)
            sleep_ms(500);
        }
        
        if (gpio_get(BTN_GRAVE) == 0) {
            add_alert("Alerta Grave Recebido!");
            printf("Alerta Grave enviado!\n");

            set_led_rgb(1, 0, 0); // Vermelho
            sleep_ms(500);
        }
        
        sleep_ms(100);
    }
    
    cyw43_arch_deinit();
    return 0;
}
