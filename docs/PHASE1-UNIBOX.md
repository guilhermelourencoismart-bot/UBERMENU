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
