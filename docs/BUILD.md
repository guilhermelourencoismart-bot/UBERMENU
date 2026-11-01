# Compilar e validar

## Windows

Visual Studio 2022, workload Desktop development with C++, MSVC v143 x64/x86,
Windows SDK, NuGet CLI e Python 3. Abrir `Unibox.sln` e selecionar Release/x64.
No x64 Native Tools Command Prompt/Developer PowerShell configurado para x64,
com MSBuild/NuGet no PATH, a partir da raiz (comandos abaixo em PowerShell):

```powershell
python scripts/validate_unibox.py
python -m unittest discover -s tests -p "test_*.py" -v
nuget restore Unibox.sln -ConfigFile NuGet.config -NonInteractive
python scripts/validate_unibox.py --require-packages
cl /nologo /EHsc /std:c++20 tests\radar_math_tests.cpp /Fe:calculation_tests.exe
.\calculation_tests.exe
msbuild Unibox.sln /m:2 /p:Configuration=Release /p:Platform=x64 /bl:msbuild-Release.binlog
python scripts/verify_pe.py output/x64/Release/Uniboxx64Release.dll
```

Interromper ao primeiro exit code diferente de zero. A checagem PE comprova
cabeçalho AMD64/PE32+/DLL, não carregamento ou funcionamento no TF2.
Boost 1.87.0 é o único pacote NuGet declarado. Não instalar vcpkg/CPR/curl/libolm
para estas adaptações. ImGui e MinHook continuam os distribuídos pela base.

## CI

`.github/workflows/msbuild.yml`: Windows Server 2022, VS 17, x64, restauração,
testes de validação, cálculos com MSVC, build e upload da DLL/PDB/binlog.
Release é o alvo necessário; as variantes originais da matriz foram preservadas.
O push da branch dispara o workflow. Também há `workflow_dispatch`.
Artefato esperado: `Uniboxx64Release`, contendo `Uniboxx64Release.dll`.

O domínio `api.github.com` deve estar aplicado à política de rede para consultar
logs/artefatos com `gh`. Sua adição foi salva em rascunho, mas o proxy continuou
negando CONNECT 403. Não remover proxy nem desabilitar TLS para contornar isso.

## Linux: verificações independentes

```bash
python3 scripts/validate_unibox.py
python3 -m unittest discover -s tests -p 'test_*.py' -v
g++ -std=c++20 -Wall -Wextra -Werror -pthread tests/radar_math_tests.cpp -o /tmp/unibox-tests
/tmp/unibox-tests
```

Esses comandos não compilam o SDK, os módulos dependentes de Windows nem a DLL.

## Validação após compilar

Em instalação de teste TF2/Windows: abrir o menu, habilitar um módulo por vez,
salvar/carregar config e binds, testar config antiga, morte/respawn, desconexão,
troca de mapa, screenshots e unload/reload. Exercitar câmera com e sem câmera
de projéteis, saúde com grupo HealthBar, caixas com grupo Box e os timers/setas
com as alternativas habilitadas. Comparar Hitscan/Projectile/SmoothVelocity com
o modo Unibox e os dois modos V2. Verificar armas especiais, Passtime, Navbot e
Followbot. Esta lista é um procedimento pendente, não um relatório de aprovação.
