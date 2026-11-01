# Pendências e bloqueios

1. **Build não comprovada.** Não há MSVC local. `gh` encontra CONNECT/403 no
   proxy para api.github.com, apesar de git push funcionar em github.com.
   O domínio está salvo no rascunho; aplicar nas configurações do ambiente,
   publicar quando solicitado pela interface e repetir a consulta dos logs.
   Não é necessário inventar credenciais ou reenviar um token em chat.
2. **ProfileStalker:** .cpp/.h originais não distribuídos. Referências, variáveis
   e integração condicional preservadas, com diagnóstico explícito. Sem stub.
3. **Paridade parcial com Comp/V2:** escopo por módulo em INTEGRATION-STATUS.md.
   O ESP principal e solvers completos continuam Unibox. Não são substituídos
   silenciosamente por versões reduzidas. Não há controles dos recursos ainda
   não implementados.
4. **EnemyCam:** usa a câmera/material original; requer teste de GPU, resize,
   materiais, unload e prioridade da câmera de projéteis. Não há validação de
   apresentação via textura ImGui nem alegação de captura privada da câmera.
5. **Pickup timers são estimativas:** intervalo configurável (10s padrão),
   somente posições conhecidas, sem garantia para servidores com lógica customizada.
6. **Supressão de duplicatas é conservadora:** se o grupo define uma barra/caixa,
   o overlay não a duplica, mesmo se outro filtro do grupo ocultar seu desenho.
7. **Parâmetros e binds:** novos registros preservam chaves antigas e usam tipos
   suportados; persistência e compatibilidade ainda precisam de teste no jogo.
8. **Concorrência:** snapshots protegem o desenho de ponteiros e usam mutex.
   Captura de entidades/eventos segue as premissas de threading existentes da
   base; não houve execução de detector de races no TF2.

Para continuar: confirmar a build Release x64 e obter logs do commit publicado;
corrigir falhas concretas sem remover recursos; executar regressões dos módulos;
só depois ampliar a migração dos solvers e completar a paridade visual restante.
Sem esses passos a entrega não é uma versão final validada.
