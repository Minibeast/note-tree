# Code Signing on macOS
This application is not code signed. To make it run on macOS, run the command below to sign the application with an ad-hoc signature for your machine.

```bash
codesign -s - -f --deep Note\ Tree.app
```