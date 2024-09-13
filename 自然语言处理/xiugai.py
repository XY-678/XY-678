import torch
import torch.nn as nn
import torch.optim as optim
from sklearn.datasets import load_breast_cancer
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
import pandas as pd
# 加载数据集
data = load_breast_cancer()
X, y = data.data, data.target
# 标准化特征
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)
y = y.reshape(-1, 1)
# 划分数据集
X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.2, random_state=42)
X_train, X_val, y_train, y_val = train_test_split(X_train, y_train, test_size=0.2, random_state=42)

# 转换为PyTorch的Tensor
X_train = torch.tensor(X_train, dtype=torch.float32)
y_train = torch.tensor(y_train, dtype=torch.float32)
X_val = torch.tensor(X_val, dtype=torch.float32)
y_val = torch.tensor(y_val, dtype=torch.float32)
X_test = torch.tensor(X_test, dtype=torch.float32)
y_test = torch.tensor(y_test, dtype=torch.float32)
# 定义神经网络模型
class SimpleNN(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(SimpleNN, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, output_size)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x):
        x = self.sigmoid(self.fc1(x))
        x = self.sigmoid(self.fc2(x))
        return x
# 训练和验证函数
def train_and_evaluate(model, optimizer, criterion, num_epochs=1000, batch_size=16):
    m = X_train.shape[0]
    for epoch in range(num_epochs):
        permutation = torch.randperm(m)
        X_train_shuffled = X_train[permutation]
        y_train_shuffled = y_train[permutation]

        for i in range(0, m, batch_size):
            X_batch = X_train_shuffled[i:i + batch_size]
            y_batch = y_train_shuffled[i:i + batch_size]

            optimizer.zero_grad()
            outputs = model(X_batch)
            loss = criterion(outputs, y_batch)
            loss.backward()
            optimizer.step()

        if epoch % 100 == 0:
            with torch.no_grad():
                val_outputs = model(X_val)
                val_loss = criterion(val_outputs, y_val)
                print(f'第 {epoch}次迭代后的训练集损失：{loss.item()}, 验证集损失: {val_loss.item()}')
# 测试函数
def test_model(model):
    with torch.no_grad():
        outputs = model(X_test)
        predictions = (outputs > 0.5).float()
        accuracy = (predictions == y_test).float().mean().item() * 100
    return accuracy
# 实验设置
input_size = X_train.shape[1]
hidden_size = 30
output_size = 1
num_epochs = 1000
# 不同的优化器
models_optimizers = {
    'SimpleNN_SGD': (SimpleNN(input_size, hidden_size, output_size), optim.SGD, 0.05),
}
criterion = nn.BCELoss()
results = {}
for key, (model, optimizer_class, lr) in models_optimizers.items():
    optimizer = optimizer_class(model.parameters(), lr=lr)
    print(f'Training {key}')
    train_and_evaluate(model, optimizer, criterion, num_epochs)
    accuracy = test_model(model)
    results[key] = accuracy
# 输出实验结果
results_df = pd.DataFrame(results,index=['Accuracy']).T
print(results_df)
