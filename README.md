[![progress-banner](https://backend.codecrafters.io/progress/claude-code/5e7def61-62cc-4e01-9fe8-fdf822d67421)](https://app.codecrafters.io/users/pmeena17?r=2qF)

This is a starting point for C++ solutions to the
["Build Your own Claude Code" Challenge](https://codecrafters.io/challenges/claude-code).

Claude Code is an AI coding assistant that uses Large Language Models (LLMs) to
understand code and perform actions through tool calls. In this challenge,
you'll build your own Claude Code from scratch by implementing an LLM-powered
coding assistant.

Along the way you'll learn about HTTP RESTful APIs, OpenAI-compatible tool
calling, agent loop, and how to integrate multiple tools into an AI assistant.

**Note**: If you're viewing this repo on GitHub, head over to
[codecrafters.io](https://codecrafters.io) to try the challenge.

# OpenRouter's API Guide

[OpenRouter's API Guide](https://openrouter.ai/docs/api/reference/overview)

# Passing the first stage

The entry point for your `claude-code` implementation is in `src/main.cpp`.
Study and uncomment the relevant code, and submit to pass the first stage:

```sh
codecrafters submit
```

# Stage 2 & beyond

Note: This section is for stages 2 and beyond.

1. Ensure you have `cmake` installed locally.
2. Run `./your_program.sh` to run your program, which is implemented in
   `src/main.cpp`.
3. Run `codecrafters submit` to submit your solution to CodeCrafters. Test
   output will be streamed to your terminal.

# Build Process

Note: cmake and vcpkg must be installed for build.

```sh
rm -rf build
./your_program.sh
```
You might need to run `export VCPKG_ROOT=/home/pmeena/Development/vcpkg` beforehand. Although I added it to ~/.bashrc, I had to manually run this in terminal before building.