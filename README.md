# SubsystemGameInstance for Unreal Engine 5

[English](#english) | [Русский](#русский)

---

<a name="english"></a>
## 🇬🇧 English

`USubsystemGameInstance` is a C++ base class designed to enable creating and extending **Game Instance Subsystems** directly inside Blueprints in Unreal Engine 5. By default, Unreal Engine does not allow creating Blueprint classes directly from `UGameInstanceSubsystem`. This class bridges that gap while adding built-in lifecycle events and a dynamic type-safe getter node.

### Features
- **Blueprintable**: Inherit your own Blueprint Subsystems (e.g., `BP_SubsystemTime`, `BP_SubsystemInventory`) directly from this class.
- **Lifecycle Events**: Automatically exposes `On Initialize` and `On Deinitialize` events to Blueprints.
- **Custom Getter Node (`Get Custom Subsystem`)**: A static Blueprint Pure node that automatically changes its return pin type based on the selected subsystem class, preventing broken wires and manual casting.

### How to Use

1. **Setup C++ Files**: Place `SubsystemGameInstance.h` and `SubsystemGameInstance.cpp` into your project's `Source/YourProjectName/` folder and compile.
2. **Create a Blueprint Subsystem**:
   - Right-click in the Content Browser -> **Blueprint Class**.
   - Search for `SubsystemGameInstance` under All Classes and select it. Name it (e.g., `BP_SubsystemTime`).
3. **Handle Initialization**:
   - Open your Blueprint Subsystem, go to the **My Blueprint** panel -> **Functions** -> **Override** -> Select **On Initialize** or **On Deinitialize**.
4. **Access in Blueprints**:
   - In any Blueprint (Character, Controller, etc.), right-click and search for **Get Custom Subsystem**.
   - In the **Subsystem Class** dropdown, select your Blueprint Subsystem (`BP_SubsystemTime`).
   - The output pin will automatically cast to your subsystem type, allowing you to access its variables and functions directly.

---

<a name="русский"></a>
## 🇷🇺 Русский

`USubsystemGameInstance` — это базовый C++ класс, созданный для того, чтобы разрешить создание и наследование **Game Instance Subsystems** прямо в Блюпринтах в Unreal Engine 5. По умолчанию движок не позволяет наследовать Блюпринты напрямую от `UGameInstanceSubsystem`. Этот класс решает данную проблему, а также добавляет встроенные события жизненного цикла и удобную динамическую ноду получения.

### Возможности
- **Поддержка Блюпринтов**: Создавайте собственные Блюпринт-сабсистемы (например, `BP_SubsystemTime`, `BP_SubsystemInventory`), наследуясь от этого класса.
- **События жизненного цикла**: Автоматически пробрасывает события `On Initialize` и `On Deinitialize` в Блюпринты.
- **Кастомная нода получения (`Get Custom Subsystem`)**: Статическая чистая нода (Blueprint Pure), которая автоматически меняет тип своего выходного пина под выбранный класс сабсистемы, избавляя от необходимости кастов и разрывов связей.

### Как использовать

1. **Добавление файлов**: Поместите `SubsystemGameInstance.h` и `SubsystemGameInstance.cpp` в папку `Source/ИмяВашегоПроекта/` и скомпилируйте проект.
2. **Создание Блюпринт-сабсистемы**:
   - Нажмите ПКМ в Content Browser -> **Blueprint Class**.
   - Во вкладке All Classes найдите `SubsystemGameInstance` и создайте дочерний класс (например, `BP_SubsystemTime`).
3. **Инициализация**:
   - Откройте вашу сабсистему, перейдите в панель **My Blueprint** -> **Functions** -> **Override** -> Выберите **On Initialize** или **On Deinitialize**.
4. **Получение в Блюпринтах**:
   - В любом Блюпринте (Персонаж, Контроллер и т.д.) вызовите ноду **Get Custom Subsystem**.
   - В выпадающем списке **Subsystem Class** выберите ваш блюпринт (например, `BP_SubsystemTime`).
   - Выходной пин автоматически примет нужный тип, позволяя напрямую вызывать ваши функции и переменные.
