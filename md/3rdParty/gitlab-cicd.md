```yaml
stages:
  - build

build:
  stage: build
  only:
    - cicd-test
  before_script:
    - echo "Before script execution..."
  script:
    - apt update
    - apt install sshpass -f -y
    - ssh-keygen -R 192.168.1.130
    - sshpass -p 'develop' scp -P 20114 develop@192.168.1.130:/home/develop/ss.sh .
  after_script:
    - echo "After script execution..."
  artifacts:
    paths:
      - build/*
    expire_in: 1 days
```

