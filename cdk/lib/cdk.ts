import type { Construct } from 'constructs';
import * as core from 'aws-cdk-lib/core'
import * as ec2 from 'aws-cdk-lib/aws-ec2';

class KeyPairStack extends core.Stack {
  keyPair: ec2.KeyPair;

  constructor(scope: Construct, id: string, props?: core.StackProps) {
    super(scope, id, props);

    this.keyPair = new ec2.KeyPair(this, 'ssh-key');
  }
}

interface WebServerProps extends core.StackProps {
  keyPair: ec2.KeyPair;
}

class WebServer extends core.Stack {
  constructor(scope: Construct, id: string, props: WebServerProps) {
    super(scope, id, props);

    const vpc = ec2.Vpc.fromLookup(this, 'vpc', {
      isDefault: true
    });

    const instance = new ec2.Instance(this, 'instance', {
      vpc,
      keyPair: props.keyPair,
      allowAllOutbound: false,
      instanceType: ec2.InstanceType.of(
        ec2.InstanceClass.T4G,
        ec2.InstanceSize.NANO
      ),
      machineImage: ec2.MachineImage.latestAmazonLinux2023({
        cpuType: ec2.AmazonLinuxCpuType.ARM_64,
      }),
      creditSpecification: ec2.CpuCredits.STANDARD,
    });

    instance.connections.allowFromAnyIpv4(ec2.Port.SSH, 'Allow ssh ingress');
    instance.connections.allowFromAnyIpv4(ec2.Port.tcp(8080), 'Allow dev http ingress');
    instance.connections.allowToAnyIpv4(ec2.Port.HTTPS, 'Allow all internet egress');

    new core.CfnOutput(this, 'public-ip', {
      value: instance.instancePublicIp,
    });
  }
}

const app = new core.App();

const env = {
  region: 'us-east-1',
  account: '670799836323',
}

const keyPairStack = new KeyPairStack(app, 'key-pair', {
  env,
});

new WebServer(app, 'web-server', {
  env,
  keyPair: keyPairStack.keyPair
});
