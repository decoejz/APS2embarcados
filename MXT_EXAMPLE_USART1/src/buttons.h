/** \brief Touch event struct */
struct botao {
	uint16_t x;
	uint16_t y;
	uint16_t size_x;
	uint16_t size_y;
	tImage *image;
	void (*p_handler)(void);
};

struct botao botaoLavagemDiaria;
struct botao botaoLavagemPesada;
struct botao botaoLavagemRapida;
struct botao botaoDireita;
struct botao botaoEsquerda;
struct botao botaoLock;
struct botao botaoUnlock;
struct botao botaoHome;
struct botao botaoPlayPause;
struct botao botaoOk;
struct botao imageNop;