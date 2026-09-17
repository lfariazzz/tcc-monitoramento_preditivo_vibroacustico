# Contribuindo

## Fluxo de branches — Git Flow

| Branch | Papel | Nasce de | Merge em |
|---|---|---|---|
| `main` | Só código em produção/release, com tag de versão (`v1.0.0`) | — | — |
| `develop` | Branch de integração — features se juntam antes de virar release | `main` | `main` (via release) |
| `feat/*` | Uma funcionalidade nova | `develop` | `develop` |
| `fix/*` | Correção de bug fora de produção | `develop` | `develop` |
| `release/*` | Preparação de versão (ajustes finais, changelog, bump de versão) | `develop` | `main` **e** `develop` |
| `hotfix/*` | Correção urgente em produção | `main` | `main` **e** `develop` |

Convenção de nomes (mesmo prefixo do Conventional Commits, ver seção abaixo):
```
feat/nome-curto-da-feature
fix/nome-curto-do-bug
release/1.0.0
hotfix/nome-do-bug
```

Toda mudança entra via Pull Request — nunca push direto em `main` ou `develop`.
PRs exigem 1 aprovação e todas as conversas resolvidas antes do merge.

## Convenção de commits

[Conventional Commits](https://www.conventionalcommits.org/):
```
feat: adiciona autenticação de usuário
fix: corrige cálculo de nota final
docs: atualiza README com instruções de setup
chore: atualiza dependências
refactor: reorganiza módulo de relatórios
```
