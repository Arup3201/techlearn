# Git Commands

## Add all changes

```
git add .
```

## Track another branch of the upstream

If `foo` is the branch on the upstream that you wanna track for pushing and pulling new changes then you can do that by following,

```
git branch -set--upstream-to upstream/foo
```

## Create a new branch

```
git checkout -b branch_name
```

## Change branch of your local git repo

```
git checkout branch_name
```

## Make new branch by copying the changes of one branch

```
git checkout -b new_branch old_branch
```

## Undo the new changes

```
git reset --hard master@{"1 hour ago"}
```
