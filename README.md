# SubsystemGameInstance for Unreal Engine 5

[English](#english) | [Русский](#русский)

---

<a name="english"></a>
## 🇬🇧 English

`USubsystemGameInstance` is a C++ base class designed to enable creating and extending **Game Instance Subsystems** directly inside Blueprints in Unreal Engine 5. By default, Unreal Engine does not allow creating Blueprint classes directly from `UGameInstanceSubsystem`. This class bridges that gap while adding built-in lifecycle events. Each Blueprint subsystem you create automatically gets its own **dedicated getter node** (e.g. `Get TimeSubsystem`) — no dropdowns, no manual casting.

### ⚠️ IMPORTANT: Module API Macro
When adding these files to your project, make sure to replace **`KINGDOMOFISRION_API`** with your own project or module API macro (e.g., `MYGAME_API`, `YOURPROJECT_API`). Otherwise, the project will fail to compile.

### Features
- **Blueprintable**: Inherit your own Blueprint Subsystems (e.g., `BP_SubsystemTime`, `BP_SubsystemInventory`) directly from this class.
- **Lifecycle Events**: Automatically exposes `On Initialize` and `On Deinitialize` events to Blueprints.
- **Dedicated getter node per subsystem**: The engine (`UK2Node_GetSubsystem`) generates a personal node for every loaded subsystem class — e.g. `Get TimeSubsystem` under the **GameInstance Subsystems** category, with a correctly typed output pin and no class dropdown.
- **Legacy dynamic getter (`Get Custom Subsystem`)**: Still included for cases where the subsystem class is selected at runtime via a variable. For static access, use the dedicated nodes.

### How to Use

1. **Setup C++ Files**: Place `SubsystemGameInstance.h` and `SubsystemGameInstance.cpp` into your project's `Source/YourProjectName/` folder.
2. **Update API Macro**: Open `SubsystemGameInstance.h` and change `KINGDOMOFISRION_API` to `YOURPROJECTNAME_API`. Compile your project.
3. **Create a Blueprint Subsystem**:
   - Right-click in the Content Browser -> **Blueprint Class**.
   - Search for `SubsystemGameInstance` under All Classes and select it. Name it (e.g., `BP_SubsystemTime`).
4. **Handle Initialization**:
   - Open your Blueprint Subsystem, go to the **My Blueprint** panel -> **Functions** -> **Override** -> Select **On Initialize** or **On Deinitialize**.
5. **Access in Blueprints (dedicated node — recommended)**:
   - Open your subsystem Blueprint once (so its class is loaded), then in any other Blueprint right-click and search for **Get TimeSubsystem** (your subsystem's name).
   - The node lives under the **GameInstance Subsystems** category, has no class pin, and its output pin is already typed as your subsystem — plug variables and functions straight in.
   - Under the hood the node compiles into the engine's `GetGameInstanceSubsystem` call with your class baked in.
6. **Make the nodes appear reliably (recommended)**:
   - The engine only generates dedicated nodes for **loaded** subsystem classes. To force-load all your subsystems at editor startup, register them once in **Project Settings -> Asset Manager -> Primary Asset Types to Scan**:
     - **Primary Asset Type**: any name, e.g. `SubsystemGameInstance`
     - **Asset Base Class**: `SubsystemGameInstance`
     - **Has Blueprint Classes**: enabled
     - **Directories**: the folder with your subsystems, e.g. `/Game/Blueprints/Subsystems`
   - After an editor restart, every subsystem Blueprint gets its own getter node automatically — no per-asset setup needed.
7. **Packaging note**:
   - A subsystem Blueprint is cooked into a packaged build only if something references it. Using its dedicated getter node anywhere creates that reference automatically, so nothing extra is needed. If you never place the node, add the subsystem to **Project Settings -> Packaging -> Additional Assets to Cook** (or reference it via Asset Manager) instead.

### Legacy: Get Custom Subsystem
`Get Custom Subsystem` (`WorldContext` + `SubsystemClass` dropdown, output auto-cast via `DeterminesOutputType`) remains available for dynamic scenarios — e.g. picking the subsystem class from a variable at runtime. For normal static access it is no longer needed.

---

<a name="русский"></a>
## 🇷🇺 Русский

`USubsystemGameInstance` — это базовый C++ класс, созданный для того, чтобы разрешить создание и наследование **Game Instance Subsystems** прямо в Блюпринтах в Unreal Engine 5. По умолчанию движок не позволяет наследовать Блюпринты напрямую от `UGameInstanceSubsystem`. Этот класс решает данную проблему, добавляет встроенные события жизненного цикла, а каждая созданная Блюпринт-сабсистема автоматически получает **персональную ноду получения** (например, `Get TimeSubsystem`) — без выпадающих списков и ручных кастов.

### ⚠️ ВАЖНО: Макрос API модуля
При добавлении этих файлов в свой проект обязательно замените **`KINGDOMOFISRION_API`** на макрос API вашего собственного проекта или модуля (например, `MYGAME_API`, `YOURPROJECT_API`). Без этого проект выдаст ошибку компиляции.

### Возможности
- **Поддержка Блюпринтов**: Создавайте собственные Блюпринт-сабсистемы (например, `BP_SubsystemTime`, `BP_SubsystemInventory`), наследуясь от этого класса.
- **События жизненного цикла**: Автоматически пробрасывает события `On Initialize` и `On Deinitialize` в Блюпринты.
- **Персональная нода на каждую сабсистему**: Движок (`UK2Node_GetSubsystem`) сам генерирует отдельную ноду для каждого загруженного класса сабсистемы — например, `Get TimeSubsystem` в категории **GameInstance Subsystems**, с уже типизированным выходным пином и без выбора класса.
- **Старая динамическая нода (`Get Custom Subsystem`)**: Осталась для случаев, когда класс сабсистемы выбирается переменной уже во время игры. Для обычного статичного доступа используйте персональные ноды.

### Как использовать

1. **Добавление файлов**: Поместите `SubsystemGameInstance.h` и `SubsystemGameInstance.cpp` в папку `Source/ИмяВашегоПроекта/`.
2. **Замена API макроса**: Откройте `SubsystemGameInstance.h` и замените `KINGDOMOFISRION_API` на макрос вашего модуля (`ИМЯПРОЕКТА_API`). Скомпилируйте проект.
3. **Создание Блюпринт-сабсистемы**:
   - Нажмите ПКМ в Content Browser -> **Blueprint Class**.
   - Во вкладке All Classes найдите `SubsystemGameInstance` и создайте дочерний класс (например, `BP_SubsystemTime`).
4. **Инициализация**:
   - Откройте вашу сабсистему, перейдите в панель **My Blueprint** -> **Functions** -> **Override** -> Выберите **On Initialize** или **On Deinitialize**.
5. **Получение в Блюпринтах (персональная нода — рекомендуется)**:
   - Один раз откройте Блюпринт сабсистемы (чтобы её класс загрузился), затем в любом другом Блюпринте через ПКМ найдите **Get TimeSubsystem** (имя вашей сабсистемы).
   - Нода лежит в категории **GameInstance Subsystems**, пина выбора класса у неё нет, а выходной пин уже нужного типа — цепляйте переменные и функции напрямую.
   - Под капотом нода компилируется в штатный вызов движка `GetGameInstanceSubsystem` с зашитым классом.
6. **Чтобы ноды появлялись всегда (рекомендуется)**:
   - Движок генерирует персональные ноды только для **загруженных** классов сабсистем. Чтобы все сабсистемы принудительно грузились при старте редактора, один раз зарегистрируйте их в **Project Settings -> Asset Manager -> Primary Asset Types to Scan**:
     - **Primary Asset Type**: любое имя, например `SubsystemGameInstance`
     - **Asset Base Class**: `SubsystemGameInstance`
     - **Has Blueprint Classes**: включено
     - **Directories**: папка ваших сабсистем, например `/Game/Blueprints/Subsystems`
   - После перезапуска редактора каждая Блюпринт-сабсистема автоматически получит свою ноду — настраивать каждый ассет отдельно не нужно.
7. **Про упаковку**:
   - Блюпринт сабсистемы попадёт в упакованную сборку, только если на него есть ссылка. Использование её персональной ноды где-либо создаёт такую ссылку автоматически — больше ничего не требуется. Если ноду вы нигде не ставите, добавьте сабсистему в **Project Settings -> Packaging -> Additional Assets to Cook** (или сошлитесь через Asset Manager).

### Старое: Get Custom Subsystem
`Get Custom Subsystem` (пин `WorldContext` + выбор `SubsystemClass`, авокаст выхода через `DeterminesOutputType`) остаётся для динамических сценариев — например, когда класс сабсистемы выбирается переменной во время игры. Для обычного статичного доступа она больше не нужна.
