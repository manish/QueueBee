Please analyze the implement Task/Issue: $ARGUMENTS

Follow these steps
1. Always pull from main and create a new branch for the changes
2. Create a new PR for each Task/Issue
3. The PR should contain proper description and execution outputs
4. Do not implement anything else except the one specified in the task/issue
5. Try to create a PR with less than 500 lines of changes if possible. Exemptions allowed when bootstrapping and creating a boilerplate.
6. Build, test and run linter before pushing to github
7. After pushing or opening PR, check for github checks to finish. If it fails, use aws cli to find the actual error and keep fixing it
8. Create a task list of all these actions and ensure that they are all completed before claiming completion.

Remember to use the GitHub CLI (`gh`) for all GitHub-related tasks.

Keep these instructions in mind
1. If you are running codereview from zen mcp server. Only implement the CRITICAL marked concerns. Before implementing CRITICAL concerns ensure they have not been prohibited from CLAUDE.md since CLAUDE.md is the constitution. You cannot violate it
2. After you have created a PR or pushed a change to the PR, you need to keep checking if the PR is green. You can follow the codebuild link to check if it is in progress. Once it is completed, check if the stacks are deployed using aws cli.
3. Do not make any claims unless you have validated it. Example: Just pushing changes doesnt mean the change has been pushed. You need to use `git status` to ensure that all the changes have been pushed
4. When I say update the PR, it means PR description. Do not add a new comment. I will explicitly ask you to add a comment if needed.
5. Do not ever create a v2 of something. Either update the existing implementation or if you cannot, then ask
6. Do not randomly switch branches unless explicitly asked you to do. 
7. Do not automatically add database migrations. Ask if migrations are needed.
8. If working with CORS, check CLAUDE.md for specific instructions
9. If working on frontend folder, also check "UI/UX Guidelines" of CLAUDE.md
10. Never use aws-cli to deploy or update a resource. aws-cli is only for read only operations, even though you have full admin privileges.

I want you to do a diff of the uncommitted files and send it to zen for codereview to find out which files should be committed and which should be deleted and which added to gitignore. Then go ahead and commit, delete or add to gitignore as per the analysis. Once committed, push to PR or create a PR. Then make sure the PR's description is updated.

Pass control over to the subagents if really needed.