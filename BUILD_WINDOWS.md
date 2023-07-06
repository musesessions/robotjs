# Building robotjs.node for Electron on Windows

- 25.2.0 is the Electron version
- 108 is the Node ABI associated with the Electron version.


Basically, we update the dependencies by force.

NOTE: If you run `npm rebuild --runtime=electron --target=25.2.0 --disturl=https://electronjs.org/headers --abi=108`
and it works out of the box, you do not need to do the below process.

```bat
;; update `nan`
npm i -D nan@latest


;; update `node-gyp` in `prebuild` and replace
npm i -D node-gyp@latest
cd node_modules\prebuild
npm i node-gyp@latest
cd ..\..

rmdir /s /q "node_modules\prebuild\node_modules\node-gyp"
xcopy /E /Y /Q /R "node_modules\node-gyp" "node_modules\prebuild\node_modules\node-gyp\"


;; update `node-abi`
cd node_modules\prebuild-install
npm install node-abi@latest
cd ..\..

;; check:
;; npm list node-gyp
;; should return
;; +-- node-gyp@9.4.0
;; `-- prebuild@9.1.1
;;   `-- node-gyp@9.4.0
;; where 9.4.0 is the latest version.
;;
;; npm list node-abi
;; should return the latest node-abi version under `prebuild-install`.

;; finally:
npm rebuild --runtime=electron --target=25.2.0 --disturl=https://electronjs.org/headers --abi=108
```