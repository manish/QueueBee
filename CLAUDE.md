# Git
- Make small logical changes, make sure it works, commit and push
- Make sure the commit message is concise but add specifics in the description
- IMPORTANT: When asked to "deploy the changes", DO NOT merge PRs. Only deploy the current branch
- Only merge PRs when explicitly asked to merge
- Before pushing to PR, build+test+eslint
- After pushing the commit, check the PR for status. If failed, fetch the failures and fix it

# Tools
  - Use ast-grep for code structure searches instead of grep when looking for:
    - Function definitions
    - Class declarations
    - Import statements
    - React components
    - TypeScript interfaces
  - Use grep only for simple text searches in non-code files


# C++ Coding requirements

- Use lowercase_snake for files:
   message.h / message.cpp
   topic_queue.h / topic_queue.cpp
   consumer_manager.h / consumer_manager.cpp

- Use CamelCase for class names:
  // File: message_queue.h
  class MessageQueue {  // CamelCase class
      // ...
  };