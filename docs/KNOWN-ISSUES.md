# Pendências e limites verificáveis

1. Build `f180e6b4` aprovada nas quatro variantes x64 em 41370128705.
   Execução no TF2 não testada; ver KNOWN-RUNTIME-RISKS.md e TEST-RESULTS.md.
2. ProfileStalker .cpp/.h originais não distribuídos (também ignorados no upstream).
   A integração condicional está preservada; não foi inventado substituto.
3. Sem MSVC local; validação da DLL depende do runner Windows-2022. Git push e
   resumos públicos funcionam; API autenticada/logs completos/download de artefatos
   foram negados pelo proxy api.github.com. Não houve contorno de rede/TLS.
   As anotações MSVC públicas foram usadas para correções verificáveis.
4. Nenhum teste TF2 ou benchmark foi executado. Isso não impede entregar uma
   candidata compilada. Pontos de observação: KNOWN-RUNTIME-RISKS.md.
5. MainESP usa seleção exclusiva Comp/grupos Unibox. Configs anteriores preservam
   o padrão Unibox; os controles Comp devem ser habilitados explicitamente.
6. Ícones e avatars usam assets nativos/caches Steam, sujeitos à disponibilidade.
   A câmera usa o mesmo render target/material da CameraWindow, com prioridade
   da câmera de projéteis. Estes caminhos precisam de observação gráfica local.

Módulos fora da seleção de ESP (comunicação/interação/câmera alternativa) estão
listados em FEATURES.md, com justificativa. Não são anunciados como integrados.
