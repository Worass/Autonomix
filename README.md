<p align="center">
  <h1 align="center">🤖 Autonomix — AI Developer for Unreal Engine</h1>
  <p align="center">
    <strong>A production-grade Unreal Engine editor plugin that uses AI to autonomously create, modify, and manage entire game projects — directly inside the editor.</strong>
  </p>
  <p align="center">
    A <a href="https://qxmp.ai"><strong>QXMP Labs</strong></a> project
  </p>
  <p align="center">
    <a href="#features">Features</a> •
    <a href="#complete-tool-reference">Tools</a> •
    <a href="#supported-providers">Providers</a> •
    <a href="#installation">Installation</a> •
    <a href="#quick-start">Quick Start</a> •
    <a href="#architecture">Architecture</a> •
    <a href="#license">License</a>
  </p>
</p>

---

## Overview

**Autonomix** brings agentic AI capabilities into the Unreal Engine editor. Instead of just generating code snippets, it operates as a fully autonomous developer — creating Blueprints via T3D injection, editing C++ source files, managing levels, materials, meshes, widgets, PCG graphs, animations, and more — all through a conversational chat interface embedded in the editor.

Think of it as **Cursor/Roo Code, but for Unreal Engine** — with deep engine integration that goes far beyond text editing.

### Key Differentiators

- **T3D Blueprint Injection** — Creates entire Blueprint node graphs in a single transaction using UE's native T3D format (the same format the editor uses for Ctrl+C/Ctrl+V). No node-by-node API calls.
- **GUID Placeholder System** — AI uses human-readable tokens (`LINK_1`, `GUID_A`, `NODEREF_Entry`) that get resolved to real engine GUIDs automatically, preserving cross-node pin links.
- **60+ AI Tools** — Not just file editing. Blueprint creation, component management, material graphs, animation systems, PCG, Enhanced Input, performance profiling, renderer settings, and more.
- **Git-Based Checkpoint System** — Shadow git repo per session lets you save, restore, and diff any point in the AI's work.
- **Agentic Tool Loop** — The AI autonomously plans, executes tools, verifies results, and iterates until the task is complete.
- **Multi-Provider Support** — Works with Anthropic Claude, OpenAI, Google Gemini, DeepSeek, Mistral, xAI, OpenRouter, Ollama, LM Studio, and any custom OpenAI-compatible endpoint.
- **Smart Context Management** — Automatic conversation condensation, sliding-window truncation, and token budget management for long sessions.
- **Fuzzy Diff Applicator** — Levenshtein-distance fuzzy matching for code edits, preventing failures from minor whitespace or formatting differences.

---

## Features

### 🎨 Editor Integration
- **Dockable Chat Panel** — Full Slate-based chat UI with real-time SSE streaming, syntax-highlighted code blocks, unified diff viewer, and inline asset previews
- **Multi-Tab Conversations** — Work on multiple tasks simultaneously with full state persistence across editor restarts
- **Context Bar** — Live progress bar showing context window usage, token counts (input/output/cache), running cost in USD, and one-click context condensation
- **Slash Commands** — Type `/new-actor`, `/fix-errors`, `/optimize`, `/create-material`, `/refactor`, `/setup-input`, `/document`, `/add-component`, `/create-interface`, `/setup-replication` for instant workflow shortcuts with autocomplete
- **@ References** — Type `@filename` or `@folder` to include files/folders in your message context, with autocomplete suggestions
- **Task Todo List** — AI maintains a live checklist of planned steps, updated as work progresses
- **Checkpoint Panel** — View all saved checkpoints, restore to any point, diff between checkpoints
- **History Panel** — Browse past conversations with token counts, cost summaries, and one-click resume
- **File Changes Panel** — See all files modified during the current session
- **Follow-Up Suggestions** — AI suggests next actions after completing a task

### 🔒 Safety & Reliability
- **Safety Gate** — Risk evaluation (Low/Medium/High/Critical) for every tool call. Protected files (`.uplugin`, `.uproject`, `.Build.cs`) are read-only for the AI regardless of mode.
- **`.autonomixignore`** — Gitignore-style pattern file to hide files/directories from the AI. Auto-created with sensible defaults on first run. Live-reloads on changes.
- **Auto-Approval System** — Configure automatic approval for read-only operations. Safety limits on consecutive requests and cumulative cost before requiring user confirmation.
- **Tool Repetition Detection** — Automatically detects and blocks infinite loops (AI repeating the same failing tool call). Configurable threshold (default: 3 consecutive identical calls).
- **Execution Journal** — Append-only audit trail of every tool action with SHA-1 pre/post file hashes for deterministic state verification. Saved to `Saved/Autonomix/ExecutionLog_YYYYMMDD.json`.
- **Backup Manager** — Automatic file backups before modifications with undo group support.
- **Error Retry Logic** — Structured retry tracking per action with configurable max attempts.
- **Code Validation** — Generated C++ code is checked against a denylist of dangerous patterns before writing.

### 🧠 Context Management (Ported from Roo Code)
- **Conversation Condensation** — When context reaches 80%, the conversation is summarized by the AI into a compact summary while preserving critical details. Auto-triggers or manual via the "Condense" button.
- **Sliding-Window Truncation** — When condensation isn't enough, non-destructive truncation tags oldest messages (preserving them for rewind) while keeping API payloads within limits.
- **Orphan Tool Result Validation** — Multi-pass validator ensures all `tool_result` blocks reference valid `tool_use` IDs before every API call, preventing HTTP 400 errors after context truncation.
- **Per-Message Environment Details** — Each message includes fresh project context (file tree, active level, selected actors, context window stats) so the AI always has current state.
- **Token Budget Management** — Configurable context token budget (default 30K) prevents project context from consuming the entire window on large projects.

### 🔀 Task Delegation
- **Sub-Task System** — AI can spawn child tasks in different modes via the `new_task` tool. Parent task pauses and resumes when the child completes with its result.
- **Max Nesting Depth** — Configurable limit (default: 5 levels) prevents infinite delegation chains.
- **Mode Switching** — Sub-tasks can run in different agent modes (General, Blueprint, Code, etc.).

### 💰 Cost Tracking
- **Per-Request Cost Calculation** — Real-time cost tracking using model-specific pricing (input tokens, output tokens, cache writes, cache reads).
- **Session Cost Display** — Running total shown in the UI header and context bar.
- **Daily Token Limits** — Optional daily token usage cap (0 = unlimited).
- **Task History** — Each completed task records total tokens and cost for budgeting.

---

## Complete Tool Reference

### 🔧 Blueprint Tools (13 tools)

| Tool | Description |
|------|-------------|
| `create_blueprint_actor` | Create Blueprint with inline components, variables, and parent class selection |
| `add_blueprint_component` | Add components to existing Blueprints via SCS |
| `add_blueprint_variable` | Add typed member variables (bool, int, float, FVector, FString, etc.) |
| `add_blueprint_function` | Create function graphs with typed input/output parameters |
| `add_blueprint_event` | Add override events (BeginPlay, Tick, Overlap, Hit, Damage, etc.) |
| `inject_blueprint_nodes_t3d` | **PRIMARY** — Inject entire node graphs via T3D text with GUID placeholder resolution |
| `get_blueprint_info` | Full state readback: components, variables, graphs, all nodes with pin details + T3D |
| `connect_blueprint_pins` | Wire pins between nodes with type validation and pin name suggestions |
| `set_blueprint_defaults` | Set CDO properties and component template values |
| `set_component_properties` | Assign meshes, transforms, collision profiles, and generic properties |
| `compile_blueprint` | Compile and return all errors/warnings |
| `add_enhanced_input_node` | Add Enhanced Input Action nodes (can't be created via T3D) |
| `verify_blueprint_connections` | Multi-pass connection audit with auto-repair + pin value diagnostics |

### 📝 C++ Tools (4 tools)

| Tool | Description |
|------|-------------|
| `create_cpp_class` | Create .h + .cpp files with code safety validation |
| `modify_cpp_file` | Rewrite existing source files |
| `trigger_compile` | Trigger Live Coding compilation |
| `regenerate_project_files` | Regenerate VS/Rider project files after adding source |

*C++ edits also use the **fuzzy diff applicator** — Levenshtein-distance matching with middle-out search prevents failures from minor whitespace, smart quote, or formatting differences.*

### 🏗️ Level & World Tools (3 tools)

| Tool | Description |
|------|-------------|
| `spawn_actor` | Spawn actors by class name or Blueprint path with transform |
| `place_light` | Place Point, Directional, Spot, or Rect lights |
| `modify_world_settings` | Set GameMode override, default pawn class, Kill Z |

### 🎭 Material Tools (2 tools)

| Tool | Description |
|------|-------------|
| `create_material` | Create materials with expression nodes and connections to material properties |
| `create_material_instance` | Create material instances with scalar/vector parameter overrides |

### 📦 Mesh Tools (3 tools)

| Tool | Description |
|------|-------------|
| `import_mesh` | Import FBX/OBJ as static or skeletal mesh |
| `import_assets_batch` | Batch import multiple files (FBX, OBJ, PNG, TGA, WAV, MP3) |
| `configure_static_mesh` | Configure Nanite, LOD generation, collision complexity, lightmap resolution |

### 🎬 Animation Tools (5 tools)

| Tool | Description |
|------|-------------|
| `create_anim_blueprint` | Create Animation Blueprints targeting a skeleton |
| `import_animation_fbx` | Import FBX animation to create AnimSequence assets |
| `assign_anim_blueprint` | Assign AnimBP to a SkeletalMeshComponent |
| `create_anim_montage` | Create AnimMontages from AnimSequences |
| `get_anim_info` | Query skeleton compatibility, list sequences and montages |

### 🖼️ Widget / UMG Tools (5 tools)

| Tool | Description |
|------|-------------|
| `create_widget_blueprint` | Create Widget Blueprints with root panel selection |
| `add_widget` | Add widgets to the hierarchy (TextBlock, Button, Image, ProgressBar, etc.) |
| `set_widget_property` | Set Text, Color, Visibility, Percent, etc. via reflection |
| `get_widget_tree` | Read full widget hierarchy with panel/leaf classification |
| `compile_widget_blueprint` | Compile and validate Widget Blueprint |

### 🌿 PCG Tools (5 tools)

| Tool | Description |
|------|-------------|
| `create_pcg_graph` | Create empty PCG graph assets |
| `attach_pcg_component` | Attach PCG component to level actors and assign graphs |
| `set_pcg_parameter` | Set exposed PCG parameters (mesh refs, density, seed, bounds) |
| `generate_pcg_local` | Trigger PCG generation with force option |
| `get_pcg_info` | Query PCG component state and generation status |

### 🎮 Enhanced Input Tools (3 tools)

| Tool | Description |
|------|-------------|
| `create_input_action` | Create Input Action assets (Boolean, Axis1D, Axis2D, Axis3D) |
| `create_input_mapping_context` | Create Input Mapping Context assets |
| `add_input_mapping` | Bind hardware keys to actions with modifiers and triggers |

### ⚡ Performance Tools (15 tools)

| Tool | Description |
|------|-------------|
| `get_performance_stats` | FPS and frame time diagnostics |
| `get_memory_stats` | Physical/virtual memory usage and peak stats |
| `run_stat_command` | Toggle stat overlays (unit, fps, gpu, scenerendering, memory) |
| `analyze_asset_sizes` | Find the largest assets inflating package size |
| `get_cvar` | Read console variable values and help text |
| `set_cvar` | Set console variables at runtime (transient) |
| `discover_cvars` | Enumerate all CVars matching a prefix (e.g., `r.Lumen`, `r.Shadow`) |
| `execute_console_command` | Execute arbitrary console commands (Trace, ProfileGPU, memreport, etc.) |
| `start_csv_profiler` | Start CSV Profiler recording to Saved/Profiling/ |
| `stop_csv_profiler` | Stop CSV Profiler and list output files |
| `read_profiling_file` | Read and analyze CSV profiler output or memreport logs |
| `get_scalability_settings` | Read current quality/scalability levels |
| `set_scalability_settings` | Override resolution, shadow, GI, reflection, texture, effects quality |
| `get_renderer_settings` | Read Project Settings > Rendering properties via reflection |
| `set_renderer_setting` | Persistently modify renderer settings (AA method, Nanite, VT, ray tracing, GI mode) |

### 🔨 Build Tools (2 tools)

| Tool | Description |
|------|-------------|
| `build_lighting` | Build lighting (Preview/Medium/High/Production quality) |
| `package_project` | Package for Win64/Linux/Mac/Android/iOS in any configuration |

### ⚙️ Settings Tools (2 tools)

| Tool | Description |
|------|-------------|
| `read_config_value` | Read INI values from DefaultEngine/DefaultGame/DefaultEditor/DefaultInput |
| `write_config_value` | Write INI values to persist configuration changes |

### 📂 Context Tools (3 tools)

| Tool | Description |
|------|-------------|
| `list_directory` | Browse project directories with file listings |
| `search_assets` | Search the UE Asset Registry by name, class, or path |
| `read_file_snippet` | Read source files with line numbers (supports .h, .cpp, .ini, .json, etc.) |

### 🔍 File Search (via search_files tool)

Regex-powered file content search with 2-line context before/after each match. Formats output in ripgrep style. Skips binary files, respects `.autonomixignore` patterns.

### 📋 Source Control Tools (4 tools)

| Tool | Description |
|------|-------------|
| `source_control_status` | Check file status in Git/Perforce |
| `source_control_checkout` | Check out files for editing |
| `source_control_add` | Mark files for add |
| `source_control_revert` | Revert files to source control state |

### ✅ Task Tools (1 tool)

| Tool | Description |
|------|-------------|
| `update_todo_list` | Maintain a step-by-step task checklist with pending/in-progress/completed status |

---

## Supported Providers

| Provider | Models | Streaming | Extended Thinking |
|----------|--------|-----------|-------------------|
| **Anthropic** | Claude Opus 4.6, Claude Sonnet 4.6, Claude Sonnet 4.5, Claude Sonnet 4, etc. | ✅ SSE | ✅ budget_tokens |
| **OpenAI** | GPT-5.4, GPT-5.3, GPT-4o, o3, o4-mini, etc. | ✅ SSE | ✅ reasoning_effort |
| **Google** | Gemini 3.x, Gemini 2.5 Pro/Flash | ✅ SSE | ✅ thinkingBudget |
| **DeepSeek** | DeepSeek V3, DeepSeek R1 | ✅ SSE | ✅ reasoning_content |
| **Mistral** | Mistral Large, Codestral, etc. | ✅ SSE | — |
| **xAI** | Grok-3, Grok-4, etc. | ✅ SSE | — |
| **OpenRouter** | Any model via OpenRouter | ✅ SSE | Varies |
| **Ollama** | Any local model | ✅ SSE | — |
| **LM Studio** | Any local model | ✅ SSE | — |
| **Custom** | Any OpenAI-compatible endpoint | ✅ SSE | — |

---

## Installation

### Prerequisites

- **Unreal Engine 5.3+** (tested on 5.3, 5.4, 5.5)
- **Visual Studio 2022** or compatible C++ compiler
- An API key from at least one supported provider
- **(Optional)** Git installed on PATH for checkpoint features

### Steps

1. **Clone the repository** into your project's `Plugins` folder:
   ```bash
   cd YourProject/Plugins
   git clone https://github.com/PRQELT/Autonomix.git
   ```

2. **Regenerate project files** (right-click `.uproject` → Generate Visual Studio project files)

3. **Build the project** — Open in your IDE and build (or launch UE which will compile automatically)

4. **Enable the plugin** — It should be enabled by default. If not: Edit → Plugins → search "Autonomix" → Enable → Restart

5. **Configure API key** — Edit → Project Settings → Plugins → Autonomix → enter your API key for your chosen provider

### Alternative: Manual Copy

1. Download the repository as a ZIP
2. Extract the contents into `YourProject/Plugins/Autonomix/`
3. Follow steps 2-5 above

---

## Quick Start

1. **Open the Autonomix panel**: Window → Autonomix (or use the toolbar button)

2. **Start chatting**: Type a request in natural language:
   ```
   Create a third-person character Blueprint with health and stamina systems.
   Add a HUD widget that displays both values as progress bars.
   ```

3. **Use slash commands** for common workflows:
   ```
   /new-actor BP_Projectile
   /fix-errors
   /optimize
   /create-material M_Lava
   /setup-input IA_Sprint
   ```

4. **Review and approve**: The AI presents each action for approval before executing. Read-only operations can be auto-approved in settings.

5. **Iterate**: The AI verifies its work, checks for compile errors, and iterates until the task is complete. You can interrupt, provide feedback, or redirect at any time.

6. **Checkpoint & Restore**: View checkpoints in the Checkpoint Panel. Restore to any previous state if the AI went in the wrong direction.

### Example Tasks

- *"Create a door Blueprint that opens when the player overlaps a trigger box, with a timeline animation"*
- *"Add a main menu widget with Play, Settings, and Quit buttons with hover effects"*
- *"Set up an Enhanced Input system with WASD movement, mouse look, and sprint"*
- *"Create a PCG graph that scatters trees on landscape with density falloff near roads"*
- *"Profile the current level and suggest optimization improvements — set up scalability presets"*
- *"Create a material that blends between snow and rock based on world-space Z height"*
- *"Import the FBX meshes from C:/Assets/ and set up Nanite + auto LODs on each one"*
- *"Create an Animation Blueprint for my character with idle/walk/run locomotion caching in EventGraph"*
- *"Refactor MyPlayerController.cpp — split the 500-line BeginPlay into smaller functions"*

---

## Architecture

Autonomix is organized into 5 modules with clear dependency boundaries:

```
Autonomix/
├── Source/
│   ├── AutonomixCore/        # Types, settings, project context, interfaces
│   │
│   ├── AutonomixLLM/         # LLM integration layer
│   │   ├── ClaudeClient       — Anthropic SSE streaming + tool calling
│   │   ├── GeminiClient       — Google AI SSE streaming + function calling
│   │   ├── OpenAICompatClient — OpenAI/DeepSeek/Mistral/xAI/OpenRouter/Ollama/LMStudio
│   │   ├── LLMClientFactory   — Provider selection + configuration
│   │   ├── ConversationManager — Message history, truncation, effective history
│   │   ├── ContextCondenser   — Conversation summarization via LLM
│   │   ├── ContextManager     — Auto-condense/truncate orchestration
│   │   ├── ToolResultValidator — Multi-pass orphan tool_result cleanup
│   │   ├── ToolSchemaRegistry — JSON tool schema loading + filtering
│   │   ├── ModelRegistry      — Model capabilities, context windows, pricing
│   │   ├── CostTracker        — Per-request and session cost calculation
│   │   ├── AutoApprovalHandler — Request count + cost limit tracking
│   │   ├── TokenCounter       — Approximate token estimation
│   │   ├── SSEParser          — Server-Sent Events stream parser
│   │   └── MCPClient          — Model Context Protocol client
│   │
│   ├── AutonomixEngine/      # Orchestration and infrastructure
│   │   ├── ActionRouter       — Tool call dispatch to executors
│   │   ├── CheckpointManager  — Git-based shadow repo checkpoints
│   │   ├── ExecutionJournal   — Append-only audit trail with file hashing
│   │   ├── BackupManager      — File backup/restore with undo groups
│   │   ├── SafetyGate         — Risk evaluation, protected files, code validation
│   │   ├── IgnoreController   — .autonomixignore pattern matching
│   │   ├── DiffApplicator     — Fuzzy multi-search-replace with Levenshtein
│   │   ├── ContextGatherer    — Project context builder (file tree, assets)
│   │   ├── EnvironmentDetails — Per-message context injection
│   │   ├── FileSearchService  — Regex file search with context lines
│   │   ├── FileContextTracker — Track which files AI has read/modified
│   │   ├── ReferenceParser    — @file and @folder reference resolution
│   │   ├── CodeStructureParser— C++ header/class structure analysis
│   │   ├── TaskDelegation     — Parent/child sub-task management
│   │   ├── TaskHistory        — Completed task archive
│   │   ├── ToolRepetitionDetector — Infinite loop detection
│   │   ├── ErrorFeedback      — Compilation error formatting + retry logic
│   │   ├── TransactionManager — Undo/redo transaction grouping
│   │   ├── SkillsManager      — Reusable workflow templates
│   │   └── SlashCommandRegistry — /command shortcuts with autocomplete
│   │
│   ├── AutonomixActions/     # Tool executors (one per domain)
│   │   ├── BlueprintActions   — T3D injection, components, variables, functions, events
│   │   ├── CppActions         — File creation, modification, Live Coding
│   │   ├── LevelActions       — Actor spawning, lights, world settings
│   │   ├── MaterialActions    — Material creation, expressions, instances
│   │   ├── MeshActions        — Import, batch import, Nanite/LOD configuration
│   │   ├── AnimationActions   — AnimBP, FBX import, montages, skeleton queries
│   │   ├── WidgetActions      — UMG widget tree building, property setting
│   │   ├── PCGActions         — Graph creation, component attachment, generation
│   │   ├── InputActions       — Enhanced Input actions, mapping contexts, bindings
│   │   ├── BuildActions       — Lighting builds, project packaging
│   │   ├── PerformanceActions — Profiling, CVars, scalability, renderer settings
│   │   ├── SettingsActions    — INI config read/write
│   │   ├── SourceControlActions — Git/Perforce file operations
│   │   ├── ContextActions     — Directory listing, asset search, file reading
│   │   └── MediaActions       — Media asset management
│   │
│   └── AutonomixUI/          # Slate widget layer
│       ├── SAutonomixMainPanel      — Central orchestrator (tabs, agentic loop, tool dispatch)
│       ├── SAutonomixChatView       — Message list with streaming support
│       ├── SAutonomixMessage        — Individual message rendering (markdown, thinking blocks)
│       ├── SAutonomixCodeBlock      — Syntax-highlighted code display with copy button
│       ├── SAutonomixDiffViewer     — Unified diff visualization
│       ├── SAutonomixInputArea      — Multi-line input with @ and / autocomplete
│       ├── SAutonomixContextBar     — Progress bar + token/cost display
│       ├── SAutonomixCheckpointPanel— Checkpoint list, restore, diff buttons
│       ├── SAutonomixHistoryPanel   — Past conversation browser
│       ├── SAutonomixTodoList       — Live task checklist
│       ├── SAutonomixPlanPreview    — AI plan display
│       ├── SAutonomixFollowUpBar    — Suggested follow-up actions
│       ├── SAutonomixFileChangesPanel — Modified files tracker
│       ├── SAutonomixAssetPreview   — Inline asset thumbnail display
│       └── SAutonomixProgress       — Operation progress overlay
│
├── Resources/
│   ├── SystemPrompt/         # AI system prompt with rules and workflow
│   └── ToolSchemas/          # 15 JSON tool definition files
│
└── Autonomix.uplugin        # Plugin descriptor (5 modules)
```

### Key Design Patterns

- **T3D Injection Pipeline** — Blueprints are created by generating T3D text (the same format UE uses for clipboard copy/paste), resolving GUID placeholders via regex, then calling `FEdGraphUtilities::ImportNodesFromText`. This gives the AI full control over node graphs in a single atomic operation.

- **Non-Destructive Truncation** — When the context window fills up, messages are tagged with `TruncationParent` (not deleted) and filtered from the effective history. This preserves full history for rewind while keeping API payloads within limits.

- **Orphan Tool Result Validation** — Before every API call, a multi-pass validator collects all `tool_use` IDs from assistant messages, then removes any `tool_result` blocks referencing unknown IDs. This prevents Claude HTTP 400 errors after context truncation.

- **Shadow Git Checkpoints** — A separate git repository in `Saved/Autonomix/Checkpoints/[SessionId]/` mirrors tracked project files. Each tool execution batch creates a commit. Restore to any checkpoint without affecting the project's own git history.

- **Fuzzy Diff Application** — Code edits use Levenshtein-distance fuzzy matching with "middle-out" search starting from hint lines. Similarity threshold of 80% prevents failures from minor whitespace or formatting differences that are common with UE's complex macros.

- **Agentic Loop with Safety Gates** — The AI runs in a loop: receive response → evaluate risk → execute tools → verify results → feed back → repeat. Safety gates detect infinite loops, tool repetition, path violations, and dangerous code patterns.

---

## Configuration

All settings are in **Edit → Project Settings → Plugins → Autonomix**:

### API Settings
| Setting | Default | Description |
|---------|---------|-------------|
| **Provider** | Anthropic | Which AI provider to use |
| **API Key** | *(per provider)* | Your provider API key (masked in UI) |
| **Model** | claude-sonnet-4-6 | Model to use for generation |
| **Max Response Tokens** | 8,192 | Maximum tokens per AI response |
| **Extended Thinking** | Off | Enable Claude's extended thinking mode |
| **Thinking Budget** | 3,000 | Token budget for extended thinking |

### Context Settings
| Setting | Default | Description |
|---------|---------|-------------|
| **Context Window** | 200K | Standard (200K) or Extended (1M) |
| **Auto-Condense** | On | Automatically condense context at threshold |
| **Condense Threshold** | 80% | Context usage % that triggers auto-condensation |
| **Context Token Budget** | 30,000 | Max tokens for project context per request |

### Safety & Cost Settings
| Setting | Default | Description |
|---------|---------|-------------|
| **Auto-Approve Read** | Off | Auto-approve read-only tool calls |
| **Max Consecutive Requests** | 0 | Auto-approval request limit (0 = unlimited) |
| **Max Cumulative Cost** | 0.0 | Auto-approval cost limit in USD (0 = unlimited) |
| **Daily Token Limit** | 0 | Daily token usage cap (0 = unlimited) |
| **Show Cost Estimates** | On | Display running cost in the UI |

### UI Settings
| Setting | Default | Description |
|---------|---------|-------------|
| **Streaming Display** | On | Token-by-token text streaming |

---

## Security

- **No hardcoded keys** — All API keys are stored in UE's per-project config system (`Saved/Config/`), excluded from version control by default.
- **Password fields** — API key inputs use UE's `PasswordField` meta tag (masked in the editor UI).
- **Protected files** — `.uplugin`, `.uproject`, `.Build.cs`, and other critical files are read-only for the AI.
- **`.autonomixignore`** — Gitignore-style file in project root controls AI file access. Auto-created with sensible defaults.
- **Safety gates** — Every tool call is evaluated for risk level before execution. Critical operations always require user approval.
- **Code validation** — Generated C++ is checked against denylist patterns before writing.
- **The `.gitignore`** excludes `Config/`, `Saved/`, `Binaries/`, `Intermediate/`, and all build artifacts.

---

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- A [**QXMP Labs**](https://qxmp.ai) project. For inquiries, contact [laurent@qxmp.ai](mailto:laurent@qxmp.ai).
- Inspired by [Roo Code](https://github.com/RooVetGit/Roo-Code) — context management, conversation condensation, checkpoint system, diff strategy, and auto-approval patterns were studied and adapted for the UE environment.
- Built with [Unreal Engine](https://www.unrealengine.com/) by Epic Games.
