# Fase 1 — preparação do Unibox

## Base e escopo

Checkout analisado: `d71471813e57eb2c6ac6f9609e48b9aa28b74866`.
Os arquivos existentes do ZIP Unibox-master coincidem com o checkout.
Nenhum módulo de Amalgam-Comp ou Amalgam V2 foi incorporado.
Não houve alteração de algoritmos, menu, serialização, ESP, Aimbot,
Misc, Navbot ou Followbot.

## ProfileStalker

- O ZIP e o checkout não contêm ProfileStalker.cpp nem ProfileStalker.h.
- Ambos estão explicitamente ignorados pelo `.gitignore`.
- A consulta ao histórico local completo desse caminho não encontrou commits.
- Não foi encontrado um arquivo renomeado correspondente. A evidência indica
  fontes não distribuídas; não permite afirmar quando ou por que foram omitidas.
- A referência no `.vcxproj` e nos filtros **já era condicional a Exists**.
  A hipótese inicial de que essa ausência necessariamente quebrava o build
  estava incorreta.
- As inclusões e chamadas no menu/Paint já estavam protegidas por
  `__has_include`. Essas proteções, referências e variáveis foram mantidas.

O novo target `ValidateOptionalProfileStalker` informa a ausência do módulo
e rejeita uma distribuição parcial (.h sem .cpp ou vice-versa). Não há stub,
remoção nem reimplementação. Para disponibilizar esse recurso, é necessário
recuperar o par original de fontes; o build público já não o oferecia.

## Arquivos alterados e motivos

- `Unibox/Unibox.vcxproj`: diagnóstico explícito do módulo opcional e erro
  antecipado para o par incompleto, sem alterar as referências existentes.
- `.github/workflows/msbuild.yml`: Windows 2022/VS 17, NuGet explícito,
  disparo manual, preflight antes/depois do restore, verificação da DLL,
  artefatos obrigatórios e log binário do build. Foram preservadas as quatro
  configurações existentes da matriz, incluindo Release x64.
- `scripts/validate_unibox.py`: validação estática de arquivos do projeto/filtros,
  par opcional, mapeamento Release x64, toolset e pacotes restaurados.
- `docs/PHASE1-UNIBOX.md`: registro desta fase e procedimento reproduzível.

## Dependências e build local Windows

Instalar Visual Studio 2022 com Desktop development with C++, MSVC v143 x64/x86
e Windows SDK 10/11 compatível com `WindowsTargetPlatformVersion=10.0`.
Manter `stdcpplatest`, ImGui e MinHook distribuídos com o Unibox.
O único pacote declarado em packages.config é Boost 1.87.0 (native).
NuGet.config usa nuget.org via HTTPS e restaura em `packages/`.
As bibliotecas Windows declaradas por pragma, como comdlg32, imagehlp,
dwmapi e windowscodecs, são fornecidas pelo SDK.

Na raiz do checkout, com MSBuild do VS2022, NuGet e Python 3 no PATH:

```powershell
python scripts/validate_unibox.py
nuget restore Unibox.sln -ConfigFile NuGet.config -NonInteractive
python scripts/validate_unibox.py --require-packages
msbuild Unibox.sln /m:2 /p:Platform=x64 /p:Configuration=Release /bl:msbuild-Release.binlog
```

Saída esperada: `output/x64/Release/Uniboxx64Release.dll` e o PDB correspondente.
Não é necessário extrair nem copiar os outros dois projetos.

Para a validação remota, publicar estas alterações na branch desejada e usar
Actions → MSBuild → Run workflow, ou o disparo normal por push/pull_request.
Este trabalho não fez push nem disparou execução remota.

> O estado acima descreve a Fase 1 inicial. A publicação posterior está
> registrada na seção Fase 1.5 abaixo.

## Evidências e limites

- Preflight executado em Linux: 1.257 referências verificadas; nenhum arquivo
  obrigatório ausente no projeto/filtros; aviso de ProfileStalker esperado.
- Validação em cópia temporária: fonte obrigatória removida gera falha;
  ProfileStalker parcial gera falha; par completo é aceito. As fontes fictícias
  desses testes só existiram na cópia temporária e foram descartadas.
- XML do projeto e YAML do workflow analisados; runner Windows 2022,
  Release na matriz e disparo manual confirmados.
- Boost 1.87.0 baixado do endpoint oficial NuGet com TLS ativo: arquivo ZIP
  sem falhas de CRC, `build/boost.targets` e
  `lib/native/include/boost/version.hpp` presentes. Isso é inspeção do pacote,
  não execução de `nuget restore` nem validação de assinatura via NuGet.
- A inspeção textual dos includes encontrou diferenças de maiúsculas em
  Followbot/FollowBot e ihandleentity/IHandleEntity, resolvíveis no Windows
  habitual. Não foram alteradas. Includes opcionais de ImGui e de plataformas
  Steam não selecionadas não são prova de erro em Release x64.
- Não há MSVC, MSBuild ou NuGet CLI nesta máquina Linux. Restore via NuGet,
  compilação, linkedição e execução do aplicativo **não foram realizados**.
- O workflow está preparado, mas ainda não foi executado com estas alterações.
  Outras falhas só poderão ser confirmadas e corrigidas após o build Windows.
- A ausência das fontes originais de ProfileStalker continua sendo uma
  limitação da distribuição, não resolvida por este diagnóstico.

## Fase 1.5 — publicação e tentativa de validação Windows

- Branch criada: `integration/phase1-unibox`, a partir da branch local `work`.
- Commit da Fase 1: `56a46a57` (`Prepare Unibox Windows x64 build validation`).
  Foram incluídos somente os quatro arquivos descritos neste documento.
- Push confirmado em `guilhermelourencoismart-bot/UBERMENU`.
- O push iniciou automaticamente o workflow MSBuild:
  https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41291735398
- O resumo público identifica o commit `56a46a5`, a branch correta e a matriz
  de quatro jobs. O workflow desse commit seleciona `windows-2022`, VS 17 e
  inclui `Release|x64`.
- O preflight local foi reexecutado com sucesso (1.257 referências); nenhuma
  alteração em `Unibox/src` foi feita.

### Bloqueio de acompanhamento

A consulta autenticada `gh run list` falhou com `Forbidden`. Um teste sem
credencial confirmou `CONNECT tunnel failed, response 403` para api.github.com:
o bloqueio ocorre no proxy antes de alcançar a API GitHub. Isso não demonstra
falha de autenticação ou erro de compilação. A página pública github.com é
acessível, mas indica que os logs exigem login.

Foi salvo no rascunho do ambiente o domínio adicional `api.github.com`,
preservando os presets existentes. Salvar o rascunho não aplica a regra à
máquina. É necessário revisar/salvar essa configuração nas configurações do
ambiente e publicar quando solicitado pela interface. Depois, repetir:

```powershell
gh run view 41291735398 --repo guilhermelourencoismart-bot/UBERMENU
gh run view 41291735398 --repo guilhermelourencoismart-bot/UBERMENU --log-failed
```

O despacho manual não chegou a ser executado: a consulta que o precedia foi
bloqueada. A execução acima foi iniciada pelo evento push, sem necessidade
de duplicá-la. Não há evidência autenticada de conclusão nem de artefato DLL
até este registro. Nome esperado: `Uniboxx64Release.dll`, no artefato
`Uniboxx64Release`; geração e arquitetura binária ainda não confirmadas.
Nenhum erro de compilador foi corrigido nesta fase, pois seus logs ainda não
estão disponíveis. A Fase 1 permanece pendente de validação real concluída;
a integração do ESP não foi iniciada.

## Evidência posterior — referência Windows aprovada

Na continuação do trabalho, a página pública da execução 41291735398,
consultada com `?attempt=1`, informou **Success**, quatro jobs concluídos,
duração 10m42s e 12 artefatos. O artefato `Uniboxx64Release` aparece com
2,51 MB e digest SHA-256
`1ff1dee2fe1cd4a0510ebc9727b7085766b9b8ec83545c03db4f7e7b2c76194c`.
Também aparecem PDB e binlog Release. O workflow desse commit exige a DLL
`Uniboxx64Release.dll` não vazia antes do upload. Isso comprova a build de
referência do commit `56a46a57`; o binário não foi baixado ou executado localmente.
Os quatro avisos visíveis são sobre migração Node.js 20/24 das actions.

A Fase 1 de compilação da referência está concluída pela evidência pública da
CI. Isso não valida a integração posterior. Os commits seguintes implementam
adaptações descritas em INTEGRATION-STATUS.md e geraram a execução
https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41293344283
para o commit `59c37e66`. A API autenticada de logs continua bloqueada pela
política de rede do ambiente.
