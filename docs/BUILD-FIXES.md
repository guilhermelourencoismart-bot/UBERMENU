# Correções verificadas durante os ports completos

As correções abaixo respondem a diferenças observadas no código ou a erros MSVC
reais. Não houve remoção de funcionalidades para satisfazer o compilador.

| Problema | Correção | Checkpoint |
|---|---|---|
| Comp usa `pSkip`; Unibox possui `m_pSkip` | Adaptados filtros de trace, mantendo máscara/entidade ignorada | c5621ccb |
| Comp usa `PlayerInfo_t`; SDK define `player_info_t` | Usado o tipo nativo | c5621ccb |
| Tags de jogador têm contratos distintos | Uso de GetTag com IDs nativos; corrigido encadeamento else e ID/index | c5621ccb, c9480149 |
| Macros globais DEG2RAD/PI não existem na base | Math::Deg2Rad/Rad2Deg e constante pi da biblioteca padrão | ce04a442, 1f84c37e |
| V2 espera hitbox infos em TickRecord | BacktrackAdapter reconstrói centros/bounds dos bones sem alterar layout nativo | ce04a442 |
| Aggregate TickRecord tem campos diferentes | Inicialização explícita de tempo/origem/bounds e cópia dos bones | ce04a442 |
| Tipos genéricos dos solvers colidem | Prefixos V2 para estruturas/enums importados | 9996e848 |
| Extração balística atingiu lambda indevida | Restaurado bloco completo do port original; alteração delimitada a CalculateAngle | 1f84c37e |
| CONTENTS_NOSKIP não existe no header nativo | Constante privada de trace V2, mesmo bit 0x80000000 da origem | 1f84c37e |
| Single/Multi não equivalem a Trace/Face nativos | V2SplashMode separado e controle aplicável ao backend, sem renomear chave nativa | ad6dd3e2 |
| IClientEntity não oferece IsPlayer | Conversão ao CBaseEntity nativo antes de consultar tipo/estado | ad6dd3e2 |
| WARNING_COLOR do SDK colide com constante CritHeals | Removidas constantes mortas do port; parâmetros continuam ConfigVar | c9480149 |
| Consultas de ownership usavam nomes anteriores dos caches | Uniformizados os membros privados no header e implementação | c2bd6d5e |
| Barreiras, caixas e barras poderiam se duplicar | Ownership por entidade realmente desenhada; backend principal exclusivo | c9480149 |
| Recursos exclusivos do Unibox ausentes no V2 | Router usa backend nativo para essas opções/armas, sem dois solvers por comando | 19ca1b7c |
| Ray/AABB paralelo podia dividir por zero | Função de interseção por slabs com guards, usada por PullPoint e testada | c9480149 |
| Pontos/segmentos/histórico inválidos poderiam crescer sem limite | Limites locais nos ports, sem reduzir opções válidas da origem | 19ca1b7c, c2758128 |
| Offscreen passa alpha como terceiro componente RGB | Quatro canais explícitos; RGB padrão da origem mantido | c2758128 |
| RT auxiliar de glyphs sem liberação no unload | Liberação na desmontagem nativa de materiais; mutex no cache de ícones | 1f84c37e |
| Cache de velocidade por índice podia reutilizar slot | Identidade inclui serial e validação de alvo; contexto invalida por arma/mapa/owner | 9ed251c7 |
| Opções de paths duplicavam configurações | Aliases às chaves nativas e controles nas categorias correspondentes | c9480149 |
| Cores expostas com NOSAVE não persistiriam | Palette Comp usa VISUAL; codec RGBA/janelas testado em produção e isoladamente | 9ed251c7, c9480149 |

Proveniência, seleção e limites de cada port: PORT-MANIFEST.json e FEATURES.md.
IDs/estados das execuções: CI-RESULTS.json. Um erro observado em uma revisão
não é automaticamente atribuído à revisão posterior que o corrige.

Linkedição: `41349672308` (`c2758128`) falhou com LNK2001/LNK1120 para os três
métodos Run V2. `f180e6b4` define nomes exclusivos Comp_/V2_ para os objetos dos
ports: arquivos de diretórios diferentes tinham o mesmo basename e eram
sobrescritos em IntDir. O preflight agora rejeita essa colisão, com teste de
regressão que cria uma colisão real de basename no projeto temporário.

`9cda5294`/`7d33cc3a`: classificação de polígonos convexos/côncavos/degenerados
compartilhada com testes; usa triangulação ImGui apropriada para pie charts
maiores que 180 graus. `bea91bf8`: guards de erro/dimensões do render target
antes de calcular o viewport, preservando o recurso compartilhado de câmera.
